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
#include <vup/Shader/Compute_shader.h>

struct MVP {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;

    void update(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) {
        model = m;
        view = v;
        projection = p;
    }
};

int main() {
    vup::init_GLFW();
    int width = 800;
    int height = 600;
    vup::Window curr_window(width, height, "IISPH demo", true);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::Trackball_camera cam(width, height);
    vup::print_context_info();
    vup::init_demo_OpenGL_params();
    auto resize_callback = [](GLFWwindow* window, int w, int h) { glViewport(0, 0, w, h); };
    resize_callback(nullptr, width, height);
    curr_window.set_resize(resize_callback);
    float delta = 0.01f;
    vup::Cube bounds_cube(2.0f, 2.0f, 2.0f);
    vup::VAO bounds_vao(bounds_cube);
    vup::V_F_shader box_renderer("../../src/shader/mvp_ubo.vert",
                                 "../../src/shader/minimal.frag");
    glm::mat4 model(1.0f);
    glm::mat4 bb_model(1.0f);
    MVP mats{glm::mat4(1.0f), cam.get_view(), cam.get_projection()};
    float radius = 0.05;
    vup::Sphere sphere(radius);
    vup::Instanced_VAO particle_spheres(sphere);
    auto particle_data = vup::create_uniform_IISPH_particles(radius, -0.6f, 0.6f,
                                                             1000.0f,
                                                             radius * 2.0f);
    vup::IISPH_demo_constants demo_consts(radius, 4.0f * radius);
    vup::SSBO demo_consts_buffer(demo_consts, 4);
    vup::SSBO particles(particle_data, 3);
    auto instances = static_cast<int>(particle_data.size());
    particle_data.clear();
    int neighbor_amount = 100;
    std::vector<int> neighbor_data(neighbor_amount * instances);
    std::vector<int> neighbor_count_data(instances);
    vup::SSBO neighbors_ssbo(neighbor_data, 5);
    vup::SSBO neighbor_count_ssbo(neighbor_count_data, 6);
    neighbor_data.clear();
    neighbor_count_data.clear();

    std::vector<float> new_densities(instances);
    vup::SSBO densities(new_densities, 7, vup::gl::Storage::read);

    std::vector<vup::Shader_define> sph_defines = {
        {"N", std::to_string(instances)},
        {"NEIGHBOR_AMOUNT", std::to_string(neighbor_amount)},
        {"NEIGHBOR_ARRAY_SIZE", std::to_string(instances * neighbor_amount)}
    };

    vup::V_F_shader particle_renderer("../../src/shader/particles/iisph/instanced_iisph.vert",
                                      "../../src/shader/particles/particles.frag",
                                      vup::gl::Introspection::ubos, sph_defines);
    particle_renderer.update_ubo("mvp", mats);

    vup::Compute_shader find_neighbors("../../src/shader/particles/iisph/find_neighbors.comp",
                                       vup::gl::Introspection::basic, sph_defines);
    find_neighbors.update_uniform("dt", delta);

    vup::Compute_shader calc_density("../../src/shader/particles/iisph/calc_density.comp",
                                     vup::gl::Introspection::basic, sph_defines);
    calc_density.update_uniform("dt", delta);

    vup::Compute_shader predict_advection("../../src/shader/particles/iisph/predict_advection.comp",
                                          vup::gl::Introspection::basic, sph_defines);
    predict_advection.update_uniform("dt", delta);

    vup::Compute_shader init_pressure_solver("../../src/shader/particles/iisph/init_pressure_solver.comp",
                                             vup::gl::Introspection::basic, sph_defines);
    init_pressure_solver.update_uniform("dt", delta);

    vup::Compute_shader calc_dijpjsum("../../src/shader/particles/iisph/calc_dijpjsum.comp",
                                      vup::gl::Introspection::basic, sph_defines);
    calc_dijpjsum.update_uniform("dt", delta);

    vup::Compute_shader solve_pressure("../../src/shader/particles/iisph/solve_pressure.comp",
                                       vup::gl::Introspection::basic, sph_defines);
    solve_pressure.update_uniform("dt", delta);

    vup::Compute_shader integrate("../../src/shader/particles/iisph/integrate.comp",
                                  vup::gl::Introspection::basic, sph_defines);
    integrate.update_uniform("dt", delta);

    vup::Compute_shader reduce_densities("../../src/shader/particles/iisph/reduce_densities.comp",
                                         vup::gl::Introspection::basic,
                                         {{"X", "128"}, {"N", std::to_string(instances)}});
    int max_blocks = static_cast<int>(glm::ceil(static_cast<float>(instances)
                                                / reduce_densities.get_workgroup_size_x()));
    reduce_densities.update_uniform("max_index", instances);
    reduce_densities.update_uniform("max_blocks", max_blocks);
    new_densities.resize(max_blocks);

    vup::Compute_shader rotate_box("../../src/shader/compute/rotate_box.comp");
    bounds_vao.get_vbo(0).bind_base(12);
    bb_model = glm::rotate(bb_model, delta * 0.1f, glm::vec3(0.3, 0.3f, 0.3f));
    rotate_box.update_uniform("model", bb_model);

    float density_avg = 0.0f;
    float density_rest = 1000.0f;
    float eta = 1.0f;

    auto loop = [&](float dt) {
        rotate_box.run(bounds_cube.vertices.size());
        cam.update(curr_window, dt);
        mats.update(model, cam.get_view(), cam.get_projection());
        find_neighbors.run_with_barrier(instances);
        calc_density.run_with_barrier(instances);
        predict_advection.run_with_barrier(instances);
        init_pressure_solver.run_with_barrier(instances);
        density_avg = 0;
        int iteration = 0;
        while ((density_avg - density_rest) > eta || iteration < 2) {
            calc_dijpjsum.run_with_barrier(instances);
            solve_pressure.run_with_barrier(instances);

            reduce_densities.run(instances);
            densities.get_data(new_densities);
            for (auto v : new_densities) {
                density_avg += v;
            }
            density_avg /= instances;
            iteration++;
            std::cout << "Density average: " << density_avg << "\n";
        }
        integrate.run_with_barrier(instances);
        particle_renderer.update_ubo("mvp", mats);
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