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
#include "vup/Shader/Compute_pipeline.h"

int main() {
    vup::init_GLFW();
    int width = 800;
    int height = 600;
    vup::Window curr_window(width, height, "IISPH demo", true);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::Trackball_camera cam(width, height);
    vup::init_demo_OpenGL_params();
    float delta = 0.001f;
    float visc_const = 0.02f;
    float tension_const = 2.0f;
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
    vup::IISPH_demo_constants demo_consts(smoothing_length, mass_scaling);
    auto particle_data = vup::create_uniform_IISPH_particles(demo_consts.r, h, -0.6f, 1.0f,
                                                             1000.0f);
    vup::Sphere sphere(demo_consts.r);
    vup::Instanced_VAO particle_spheres(sphere);
    vup::SSBO particles(particle_data, 0);
    vup::SSBO demo_consts_buffer(demo_consts, 1);
    auto instances = static_cast<int>(particle_data.size());
    particle_data.clear();
    int neighbor_amount = 100;
    vup::Empty_SSBO neighbors_ssbo(neighbor_amount * instances * sizeof(int), 2);
    vup::Empty_SSBO neighbor_count_ssbo(instances * sizeof(int), 3);
    std::vector<float> new_densities(instances);
    vup::SSBO densities(new_densities, 6, vup::gl::storage::read);

    gl_debug_logger.retrieve_log(std::cout);

    std::vector<vup::Shader_define> sph_defines = {
        {"N", std::to_string(instances)},
        {"NEIGHBOR_AMOUNT", std::to_string(neighbor_amount)},
        {"NEIGHBOR_ARRAY_SIZE", std::to_string(instances * neighbor_amount)}
    };

    vup::V_F_shader particle_renderer("../../src/shader/particles/iisph/instanced_iisph.vert",
                                      "../../src/shader/particles/particles.frag",
                                      vup::gl::introspection::basic, sph_defines);

    vup::UBO mvp(mats, 8);

    gl_debug_logger.retrieve_log(std::cout);

    vup::Compute_pipeline prepare_iteration({
                                                "find_neighbors.comp", "calc_density.comp",
                                                "predict_advection.comp", "init_pressure_solver.comp"
                                            },
                                            vup::gl::introspection::basic, sph_defines,
                                            "../../src/shader/particles/iisph/");
    prepare_iteration.update_uniform("dt", delta);
    prepare_iteration.update_uniform_at(2, "visc_const", visc_const);
    prepare_iteration.update_uniform_at(2, "tension_const", tension_const);

    vup::Compute_pipeline iisph_iteration({"calc_dijpjsum.comp", "solve_pressure.comp"},
                                          vup::gl::introspection::basic, sph_defines,
                                          "../../src/shader/particles/iisph/");
    iisph_iteration.update_uniform("dt", delta);

    vup::Compute_shader integrate("../../src/shader/particles/iisph/integrate.comp",
                                  vup::gl::introspection::basic, sph_defines);
    integrate.update_uniform("dt", delta);

    vup::Compute_shader reduce_densities("../../src/shader/particles/iisph/reduce_densities.comp",
                                         vup::gl::introspection::basic,
                                         {{"X", "1024"}, {"N", std::to_string(instances)}});
    int max_blocks = static_cast<int>(glm::ceil(static_cast<float>(instances)
                                                / reduce_densities.get_workgroup_size_x()));
    reduce_densities.update_uniform("max_index", instances);
    reduce_densities.update_uniform("max_blocks", max_blocks);
    new_densities.resize(max_blocks);

    vup::Compute_shader rotate_box("../../src/shader/compute/rotate_box.comp");
    bounds_vao.get_vbo(0).bind_base(10);
    bb_model = rotate(bb_model, delta * 0.1f, glm::vec3(0.3, 0.3f, 0.3f));
    rotate_box.update_uniform("model", bb_model);

    float density_avg = 0.0f;
    float density_rest = 1000.0f;
    float eta = 1.0f;

    const auto loop = [&](float dt) {
        rotate_box.run(bounds_cube.vertices.size());
        cam.update(curr_window, dt);
        mats.update(model, cam.get_view(), cam.get_projection());
        prepare_iteration.run_with_barrier(instances);
        density_avg = 0;
        int iteration = 0;
        while ((density_avg - density_rest) > eta || iteration < 2) {
            iisph_iteration.run_with_barrier(instances);

            reduce_densities.run_with_barrier(instances);
            densities.get_data(new_densities);
            for (auto v : new_densities) {
                density_avg += v;
            }
            density_avg /= instances;
            iteration++;
        }
        integrate.run_with_barrier(instances);
        mvp.update_data(mats);
        particle_renderer.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        particle_spheres.render(GL_TRIANGLES,
                                static_cast<unsigned int>(instances));
        box_renderer.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        bounds_vao.render(GL_TRIANGLES);
        gl_debug_logger.retrieve_log(std::cout);
    };
    curr_window.run_loop_fixed(delta, loop);
    glfwTerminate();
    return 0;
}
