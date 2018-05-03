//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Simulation/particle_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Shader/V_F_shader.h>
#include <vup/Shader/V_G_F_shader.h>
#include <vup/GPU_Storage/Instanced_VAO.h>
#include <vup/GPU_Storage/VAO.h>
#include <vup/Utility/OpenGL_debug_logger.h>
#include <vup/Shader/Compute_pipeline.h>
#include <vup/Simulation/Acceleration_structure.h>
#include <vup/Core/Gui_window.h>

int main() {
    vup::init_glfw();
    const int width = 800;
    const int height = 600;
    vup::Gui_window curr_window(width, height, "IISPH demo", true);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::Trackball_camera cam(width, height);
    vup::init_demo_OpenGL_params();
    float delta = 0.01f;
    float visc_const = 0.02f;
    float tension_const = 0.2f;
    vup::Cube bounds_cube(2.0f, 2.0f, 2.0f);
    vup::VAO bounds_vao(bounds_cube);
    vup::V_F_shader box_renderer("../../src/shader/rendering/mvp_ubo.vert",
                                 "../../src/shader/rendering/minimal.frag");
    glm::mat4 model(1.0f);
    glm::mat4 bb_model(1.0f);
    vup::MVP mats{glm::mat4(1.0f), cam.get_view(), cam.get_projection()};
    float smoothing_length = 0.1;
    float mass_scaling = 2.0f / 3.0f;
    float h = smoothing_length * mass_scaling;

    float density_avg = 0.0f;
    float density_rest = 1000.0f;
    float eta = 1.0f;
    vup::IISPH_demo_constants demo_consts(smoothing_length, mass_scaling);
    auto particle_data = vup::create_uniform_IISPH_particles(demo_consts.r, h, -0.6f, 1.0f,
                                                             density_rest);
    const vup::Sphere sphere(demo_consts.r);
    vup::Instanced_VAO particle_spheres(sphere);
    vup::SSBO particles(particle_data, 0);
    vup::SSBO demo_consts_buffer(demo_consts, 1);
    auto instances = static_cast<int>(particle_data.size());
    int neighbor_amount = 40;
    vup::Empty_SSBO neighbors_ssbo(neighbor_amount * instances * sizeof(int), 2);
    vup::Empty_SSBO neighbor_count_ssbo(instances * sizeof(int), 3);

    vup::Cube_uniform_grid_params grid_params(4.0f, demo_consts.h, 40);
    vup::Empty_SSBO uniform_grid_ssbo(grid_params.grid_capacity * sizeof(int), 4);
    vup::Empty_SSBO uniform_grid_cell_counter_ssbo(grid_params.total_cell_count * sizeof(int), 5);
    vup::SSBO grid_params_buffer(grid_params, 6);

    std::vector<float> new_densities(instances);
    vup::SSBO densities(new_densities, 7, vup::gl::storage::read);

    const std::vector<vup::Shader_define> sph_defines = {
        {"N", std::to_string(instances)},
        {"NEIGHBOR_AMOUNT", std::to_string(neighbor_amount)},
        {"NEIGHBOR_ARRAY_SIZE", std::to_string(instances * neighbor_amount)},
        {"GRID_CAPACITY", std::to_string(grid_params.grid_capacity)},
        {"CELL_COUNT", std::to_string(grid_params.total_cell_count)}
    };

    vup::V_F_shader particle_renderer("../../src/shader/particles/iisph/instanced_iisph.vert",
                                      "../../src/shader/particles/particles.frag", sph_defines);

    vup::UBO mvp(mats, 8);

    vup::Compute_shader reset_grid("../../src/shader/data_structures/reset_grid.comp", sph_defines);
    vup::Compute_pipeline init_iteration({
                                             "populate_grid.comp", "find_neighbors_in_grid.comp",
                                             "calc_density.comp", "predict_advection.comp",
                                             "init_pressure_solver.comp"
                                         },
                                         sph_defines,
                                         "../../src/shader/particles/iisph/");
    init_iteration.update_uniform("dt", delta);
    init_iteration.update_uniform("visc_const", visc_const);
    init_iteration.update_uniform("tension_const", tension_const);

    vup::Compute_pipeline pressure_solver({"calc_dijpjsum.comp", "solve_pressure.comp"},
                                          sph_defines,
                                          "../../src/shader/particles/iisph/");
    pressure_solver.update_uniform("dt", delta);

    vup::Compute_shader integrate("../../src/shader/particles/iisph/integrate.comp", sph_defines);
    integrate.update_uniform("dt", delta);

    vup::Compute_shader reduce_densities("../../src/shader/particles/iisph/reduce_densities.comp",
                                         {{"X", "1024"}, {"N", std::to_string(instances)}});
    const auto max_blocks = static_cast<int>(glm::ceil(static_cast<float>(instances)
                                                       / reduce_densities.get_workgroup_size_x()));
    reduce_densities.update_uniform("max_index", instances);
    reduce_densities.update_uniform("max_blocks", max_blocks);
    new_densities.resize(max_blocks);

    vup::Compute_shader rotate_box("../../src/shader/compute/rotate_box.comp");
    bounds_vao.get_vbo(0).bind_base(10);
    bb_model = rotate(bb_model, delta * 0.1f, glm::vec3(0.3, 0.3f, 0.3f));
    rotate_box.update_uniform("model", bb_model);

    bool pause_sim = false;
    const auto loop = [&](float dt) {
        ImGui::Begin("My First Tool");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        if (ImGui::SliderFloat("Smoothing length", &smoothing_length, 0.0f, 1.0f)) {
            demo_consts = vup::IISPH_demo_constants(smoothing_length, mass_scaling);
            demo_consts_buffer.update_data(demo_consts);
        }
        if (ImGui::SliderFloat("Viscosity", &visc_const, 0.0f, 1.0f)) {
            init_iteration.update_uniform("visc_const", visc_const);
        }
        if (ImGui::SliderFloat("Surface Tension", &tension_const, 0.0f, 10.0f)) {
            init_iteration.update_uniform("tension_const", tension_const);
        }
        ImGui::SliderFloat("Density difference", &eta, 0.0f, 10.0f);
        if (!pause_sim && ImGui::Button("Pause simulation")) {
            pause_sim = true;
        }
        if (pause_sim && ImGui::Button("Unpause simulation")) {
            pause_sim = false;
        }
        if (ImGui::Button("Reload scene")) {
            particles.update_data(particle_data);
            demo_consts_buffer.update_data(demo_consts);
            particle_renderer.reload();
            box_renderer.reload();
            rotate_box.reload();
            reset_grid.reload();
            init_iteration.reload();
            pressure_solver.reload();
            bounds_vao.get_vbo(0).bind_base(10);
            rotate_box.update_uniform("model", bb_model);
            integrate.update_uniform("dt", dt);
            pressure_solver.update_uniform("dt", dt);
            init_iteration.update_uniform("dt", dt);
            init_iteration.update_uniform("tension_const", tension_const);
            init_iteration.update_uniform("visc_const", visc_const);
        }
        ImGui::End();
        if (!pause_sim) {
            rotate_box.run(bounds_cube.vertices.size());
            reset_grid.run_with_barrier(grid_params.total_cell_count);
            init_iteration.run_with_barrier(instances);
            density_avg = 0;
            int iteration = 0;
            while ((density_avg - density_rest > eta || iteration < 2) && iteration < 15) {
                pressure_solver.run_with_barrier(instances);

                reduce_densities.run_with_barrier(instances);
                densities.get_data(new_densities);
                for (auto v : new_densities) {
                    density_avg += v;
                }
                density_avg /= instances;
                iteration++;
            }
            integrate.run_with_barrier(instances);
        }
        cam.update(curr_window, dt);
        mats.update(model, cam.get_view(), cam.get_projection());
        mvp.update_data(mats);
        particle_renderer.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        particle_spheres.render(GL_TRIANGLES, static_cast<unsigned int>(instances));
        box_renderer.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        bounds_vao.render(GL_TRIANGLES);
        gl_debug_logger.retrieve_log(std::cout);
    };
    curr_window.run_loop_fixed(delta, loop);
    return 0;
}
