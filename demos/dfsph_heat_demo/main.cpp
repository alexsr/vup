//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Simulation/particle_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Shader/Shader.h>
#include <vup/GPU_Storage/VAO.h>
#include <vup/Utility/OpenGL_debug_logger.h>
#include <vup/Shader/Compute_pipeline.h>
#include <vup/Core/Gui_window.h>
#include <vup/Simulation/simulation_utils.h>
#include <vup/Simulation/acceleration_structures.h>
#include "vup/Simulation/compute_utils.h"

int main() {
    vup::init_glfw();
    const int width = 800;
    const int height = 600;
    vup::Gui_window curr_window(width, height, "DFSPH heat transfer demo", true);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::Trackball_camera cam(width, height);
    vup::init_demo_OpenGL_params();
    const vup::Cube bounds_cube(2.0f, 2.0f, 2.0f);
    vup::VAO bounds_vao(bounds_cube);
    vup::V_F_shader box_renderer("../../src/shader/rendering/mvp_ubo.vert",
                                 "../../src/shader/rendering/minimal.frag");
    glm::mat4 model(1.0f);
    glm::mat4 bb_model(1.0f);
    vup::MVP mats{glm::mat4(1.0f), cam.get_view(), cam.get_projection()};
    float h = 0.1f;
    float mass_scaling = 2.0f / 3.0f;

    vup::Simulation_timer sim_timer;
    sim_timer.time_scaling = 1.0;
    sim_timer.dt = 0.0001f;
    float density_rest = 1000.0f; // density at 4C
    float visc_const = 500000.0;
    float tension_const = 0.0f;
    float temperature = 0.0f;
    float max_error = 0.1f;
    float cg_max_error = 0.01f;
    float density_eta = density_rest * max_error * 0.01f;
    float div_eta = density_eta * 1.0f / sim_timer.dt;
    int max_iterations = 100;
    float heat_const = 0.591f / 4181.3f;
    float latent_heat_max = 100.0f;
    float heat_source_temp = 100.0f;
    vup::DFSPH_heat_demo_constants demo_consts(h, mass_scaling, sim_timer.dt);
    vup::DFSPH_gen_settings particle_settings(demo_consts.r, -0.5f, 0.5f, mass_scaling, density_rest, visc_const,
                                              temperature, heat_const, latent_heat_max);
    vup::SSBO particle_settings_ubo(particle_settings, 15);
    //    auto particle_data = vup::create_DFSPH_heat_particles(demo_consts.r, h, mass_scaling, -0.5f, 0.5f,
    //                                                          density_rest, visc_const, temperature, 100.0f);
    auto instances = static_cast<int>(particle_settings.res * particle_settings.res * particle_settings.res);
    const vup::Sphere sphere(demo_consts.r);
    vup::Instanced_VAO particle_spheres(sphere);
    vup::Empty_SSBO particles(instances * sizeof(vup::DFSPH_heat_particle), 0,
                              vup::gl::storage::dynamic | vup::gl::storage::read | vup::gl::storage::write);
    vup::SSBO demo_consts_buffer(demo_consts, 1);
    const int neighbor_amount = 40;
    vup::Empty_SSBO neighbors_ssbo(neighbor_amount * instances * sizeof(int), 2);
    vup::Empty_SSBO neighbor_count_ssbo(instances * sizeof(int), 3);

    vup::Cube_uniform_grid_params grid_params(3.0f, demo_consts.h, neighbor_amount);
    vup::Empty_SSBO uniform_grid_ssbo(grid_params.grid_capacity * sizeof(int), 4);
    vup::Empty_SSBO uniform_grid_cell_counter_ssbo(grid_params.total_cell_count * sizeof(int), 5);
    vup::SSBO grid_params_buffer(grid_params, 6);

    auto scalar_buffer = std::make_shared<vup::Empty_SSBO>(instances * sizeof(float), 7, vup::gl::storage::read);

    const auto boundary_data = vup::create_boundary_box(glm::vec3(2.0f), glm::vec4(0.0), demo_consts.r);
    vup::SSBO boundary(boundary_data, 8);

    const std::vector<vup::Shader_define> sph_defines = {
        {"N", std::to_string(instances)},
        {"B", std::to_string(boundary_data.size())},
        {"NEIGHBOR_AMOUNT", std::to_string(neighbor_amount)},
        {"NEIGHBOR_ARRAY_SIZE", std::to_string(instances * neighbor_amount)},
        {"GRID_CAPACITY", std::to_string(grid_params.grid_capacity)},
        {"CELL_COUNT", std::to_string(grid_params.total_cell_count)}
    };

    vup::UBO mvp(mats, 0);

    vup::V_F_shader particle_renderer("../../src/shader/particles/dfsph/heat_particle.vert",
                                      "../../src/shader/particles/particles.frag", sph_defines);

    vup::V_F_shader boundary_renderer("../../src/shader/particles/dfsph/instanced_boundary.vert",
                                      "../../src/shader/particles/particles.frag", sph_defines);

    vup::Compute_shader gen_particles("../../src/shader/particles/dfsph/generate_particles.comp", sph_defines);

    vup::Compute_shader reset_grid("../../src/shader/data_structures/reset_grid.comp", sph_defines);
    vup::Compute_shader calc_boundary_psi("../../src/shader/particles/dfsph/calc_boundary_psi.comp", sph_defines);

    vup::Compute_pipeline calc_density_alpha({
                                                 "populate_grid.comp", "find_neighbors_in_grid.comp",
                                                 "calc_density_alpha.comp"
                                             },
                                             sph_defines,
                                             "../../src/shader/particles/dfsph/");

    vup::Compute_pipeline correct_divergence_error({
                                                       "predict_density_div.comp",
                                                       "adapt_vel_div.comp"
                                                   },
                                                   sph_defines,
                                                   "../../src/shader/particles/dfsph/divergence_solver/");

    vup::Compute_shader compute_accel("../../src/shader/particles/dfsph/calc_non_pressure_accel.comp", sph_defines);
    vup::Compute_shader update_velocities("../../src/shader/particles/dfsph/update_velocities.comp", sph_defines);

    vup::Compute_pipeline correct_density_error({
                                                    "predict_density.comp",
                                                    "adapt_vel_density.comp"
                                                },
                                                sph_defines,
                                                "../../src/shader/particles/dfsph/density_solver/");

    vup::Compute_pipeline update_positions({"calc_heat_transfer.comp", "update_positions.comp"},
                                           sph_defines, "../../src/shader/particles/dfsph/");

    vup::Compute_pipeline initiate_viscosity_solver({
                                                        "jacobi_preconditioner.comp",
                                                        "calc_initial_residual.comp"
                                                    },
                                                    sph_defines,
                                                    "../../src/shader/particles/dfsph/viscosity/");

    vup::Compute_shader initial_precond_solve("../../src/shader/particles/dfsph/viscosity/initial_precond_solve.comp",
                                              sph_defines);

    vup::Compute_shader compute_Ap("../../src/shader/particles/dfsph/viscosity/compute_Ap.comp",
                                   sph_defines);
    vup::Compute_shader calc_residual_squared("../../src/shader/particles/dfsph/viscosity/calc_residual_squared.comp",
                                              sph_defines);
    vup::Compute_shader update_solution("../../src/shader/particles/dfsph/viscosity/update_solution.comp", sph_defines);

    vup::Compute_shader precond_solve("../../src/shader/particles/dfsph/viscosity/precond_solve.comp",
                                      sph_defines);

    vup::Compute_shader update_search_dir("../../src/shader/particles/dfsph/viscosity/update_search_direction.comp",
                                          sph_defines);

    vup::Reduction reduce_scalar("../../src/shader/particles/reduce_scalar.comp", scalar_buffer, instances);
    vup::Compute_shader max_scalar("../../src/shader/particles/max_scalar.comp",
                                   {{"X", "512"}, {"N", std::to_string(instances)}});
    const auto reduce_instances = static_cast<unsigned int>(glm::ceil(instances / 2.0f));
    const auto max_blocks = static_cast<int>(reduce_instances / static_cast<float>(max_scalar.get_workgroup_size_x()));
    max_scalar.update_uniform("max_index", instances);
    max_scalar.update_uniform("max_blocks", max_blocks);
    std::vector<float> new_scalar(max_blocks);

    //bb_model = rotate(bb_model, delta * 0.1f, glm::vec3(0.3, 0.3f, 0.3f));

    initiate_viscosity_solver.update_uniform("visc_const", visc_const);
    compute_Ap.update_uniform("visc_const", visc_const);

    gen_particles.run_with_barrier(instances);

    const auto update_demo_consts = [&]() {
        demo_consts = vup::DFSPH_heat_demo_constants(h, mass_scaling, sim_timer.dt);
        demo_consts_buffer.update_data(demo_consts);
    };

    const auto reload_shaders = [&]() {
        particle_renderer.reload();
        box_renderer.reload();
        gen_particles.reload();
        reset_grid.reload();
        calc_boundary_psi.reload();
        calc_density_alpha.reload();
        correct_divergence_error.reload();
        initiate_viscosity_solver.reload();
        initial_precond_solve.reload();
        calc_residual_squared.reload();
        compute_Ap.reload();
        update_solution.reload();
        precond_solve.reload();
        update_search_dir.reload();
        compute_accel.reload();
        update_velocities.reload();
        correct_density_error.reload();
        update_positions.reload();
        max_scalar.reload();
    };

    bool pause_sim = false;
    vup::Time_counter fps_counter;
    vup::Time_counter ips_counter;
    const auto loop = [&]() {
        ImGui::Begin("My First Tool");
        ImGui::TextWrapped("Simulation average %.3f ms/iteration (%.1f Iterations per second)",
                           1000.0f / ips_counter.average_aps(), ips_counter.average_aps());
        ImGui::TextWrapped("Simulation current %.3f ms/iteration (%.1f Iterations per second)",
                           1000.0f / ips_counter.one_second_average, ips_counter.one_second_average);
        ImGui::TextWrapped("Rendering average %.3f ms/frame (%.1f FPS)",
                           1000.0f / fps_counter.average_aps(), fps_counter.average_aps());
        if (ImGui::DragFloat("Time scaling", &sim_timer.time_scaling, 0.01f, 0.01f, 10.0f)
            || ImGui::DragFloat("Iteration time", &sim_timer.dt, 0.0001f, 0.0001f, 1.0f, "%.8f")) {
            update_demo_consts();
        }
        if (ImGui::SliderFloat("Smoothing length", &h, 0.0f, 1.0f)) {
            update_demo_consts();
        }
        if (ImGui::SliderFloat("Viscosity", &visc_const, 0.0f, 100000.0f)) {
            initiate_viscosity_solver.update_uniform("visc_const", visc_const);
            compute_Ap.update_uniform("visc_const", visc_const);
        }
        if (ImGui::SliderFloat("Surface Tension", &tension_const, 0.0f, 10.0f)) {
            compute_accel.update_uniform("tension_const", tension_const);
        }
        //        if (ImGui::SliderFloat("Heat source temperature", &heat_source_temp, -1000.0f, 1000.0f)) {
        //            demo_consts.temperature = heat_source_temp + 273.15f;
        //            demo_consts_buffer.update_data(demo_consts);
        //        }
        if (pause_sim && ImGui::Button("Unpause simulation")) {
            pause_sim = false;
        }
        if (ImGui::Button("Reload scene")) {
            reload_shaders();
            particle_settings_ubo.update_data(particle_settings);
            gen_particles.run_with_barrier(instances);
            demo_consts_buffer.update_data(demo_consts);
            compute_accel.update_uniform("tension_const", tension_const);
            max_scalar.update_uniform("max_index", instances);
            max_scalar.update_uniform("max_blocks", max_blocks);
            initiate_viscosity_solver.update_uniform("visc_const", visc_const);
            compute_Ap.update_uniform("visc_const", visc_const);
            reset_grid.run_with_barrier(grid_params.total_cell_count);
            calc_density_alpha.run_with_barrier(instances);
        }
        ImGui::End();
        if (!pause_sim) {
            //while (sim_timer.is_iteration_due()) {
            reset_grid.run_with_barrier(grid_params.total_cell_count);
            calc_boundary_psi.run_with_barrier(static_cast<unsigned int>(boundary_data.size()));
            calc_density_alpha.run_with_barrier(instances);

            // divergence error correction
            float density_div_avg = 0;
            int iteration_div = 0;
            while ((density_div_avg > div_eta || iteration_div < 1) && iteration_div < max_iterations) {
                correct_divergence_error.run_with_barrier(instances);
                density_div_avg = reduce_scalar.execute<float>(std::plus<>()) / instances;
                iteration_div++;
            }

            // implicit viscosity solver
            // initiate conjugate gradient solver
            initiate_viscosity_solver.run_with_barrier(instances);
            float rhs_norm2 = reduce_scalar.execute<float>(std::plus<>());
            calc_residual_squared.run_with_barrier(instances);
            float residual_norm2 = reduce_scalar.execute<float>(std::plus<>());
            int iterations = 0;
            float cg_threshold = cg_max_error * cg_max_error * rhs_norm2;
            if (rhs_norm2 != 0 && residual_norm2 >= cg_threshold) {
                float abs_r = 0.0f;
                initial_precond_solve.run_with_barrier(instances);
                abs_r = reduce_scalar.execute<float>(std::plus<>());
                float abs_r_old = 0.0f;
                for (int it = 0; it < max_iterations; it++) {
                    iterations++;
                    float alpha_denom = 0.0f;
                    compute_Ap.run_with_barrier(instances);
                    alpha_denom = reduce_scalar.execute<float>(std::plus<>());
                    float alpha = abs_r / alpha_denom;
                    if (alpha_denom == 0) {
                        alpha = 0.0f;
                    }
                    update_solution.update_uniform("alpha", alpha);
                    update_solution.run_with_barrier(instances);
                    calc_residual_squared.run_with_barrier(instances);
                    residual_norm2 = 0;
                    residual_norm2 = reduce_scalar.execute<float>(std::plus<>());
                    if (residual_norm2 < cg_threshold) {
                        break;
                    }
                    abs_r_old = abs_r;
                    abs_r = 0.0f;
                    precond_solve.run_with_barrier(instances);
                    abs_r = reduce_scalar.execute<float>(std::plus<>());
                    float beta = abs_r / abs_r_old;
                    if (abs_r_old == 0) {
                        beta = 0.0f;
                    }
                    update_search_dir.update_uniform("beta", beta);
                    update_search_dir.run_with_barrier(instances);
                }
            }
            //            float tol_error = sqrt(residual_norm2 / rhs_norm2);
            //            std::cout << "Total error: " << tol_error << " in " << iterations << " iterations\n";
            compute_accel.run_with_barrier(instances);
            max_scalar.run_with_barrier(reduce_instances);
            scalar_buffer->get_data(new_scalar);
            float max_vel = 0;
            for (auto v : new_scalar) {
                if (max_vel < v) {
                    max_vel = v;
                }
            }
            sim_timer.update_dt_cfl(0.4f, demo_consts.h, glm::sqrt(max_vel));
            update_demo_consts();
            update_velocities.run_with_barrier(instances);
            // density error correction
            int iteration_density = 0;
            float density_avg = 0;
            while ((density_avg - density_rest > density_eta || iteration_density < 2) && iteration_density <
                   max_iterations) {
                correct_density_error.run_with_barrier(instances);
                density_avg = reduce_scalar.execute<float>(std::plus<>()) / instances;
                iteration_density++;
            }
            update_positions.run_with_barrier(instances);

            sim_timer.advance();
            ips_counter.advance();
        }
        //sim_timer.advance_accumulator(fps_counter.delta_time());
        //}
        cam.update(curr_window, fps_counter.delta_time());
        mats.update(model, cam.get_view(), cam.get_projection());
        mvp.update_data(mats);
        particle_renderer.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        particle_spheres.render(GL_TRIANGLES, static_cast<unsigned int>(instances));
        //        boundary_renderer.use();
        //        particle_spheres.render(GL_TRIANGLES, boundary_data.size());
        box_renderer.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        bounds_vao.render(GL_TRIANGLES);
        //gl_debug_logger.retrieve_log(std::cout);
        fps_counter.advance();
    };
    curr_window.run_loop(loop);
    return 0;
}
