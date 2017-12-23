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
#include <vup/Geometry/Mesh_loader.h>
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
    vup::Window window(width, height, "Calculate mesh bounding box", true);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::Trackball_camera cam(width, height);
    vup::print_context_info();
    vup::init_demo_OpenGL_params();
    auto resize_callback = [](GLFWwindow* window, int w, int h) { glViewport(0, 0, w, h); };
    resize_callback(nullptr, width, height);
    window.set_resize(resize_callback);
    bool allow_reset = true;
    float delta = 0.001f;
    vup::Cube bounds_cube(2.0f, 2.0f, 2.0f);
    vup::VAO bounds_vao(bounds_cube);
    vup::V_F_shader box_renderer("../../src/shader/mvp_ubo.vert",
                                      "../../src/shader/minimal.frag");
    glm::mat4 model(1.0f);
    glm::mat4 bb_model(1.0f);
    MVP mats{glm::mat4(1.0f), cam.get_view(), cam.get_projection()};
    vup::Instanced_VAO particle_spheres(vup::Sphere(0.05f));
    auto particle_data = vup::fill_uniformly(0.05f, -0.6f, 0.6f, 1000.0f);
    vup::SSBO particles(particle_data, 8);
    auto instances = static_cast<int>(particle_data.size());
    vup::V_F_shader particle_shader("../../src/shader/particles/instanced_sph.vert", "../../src/shader/particles/particles.frag",
                                    vup::gl::Introspection::ubos, {{"N", instances}});
    particle_shader.update_ubo("mvp", mats);
    vup::Compute_shader sph("../../src/shader/particles/sph.comp", vup::gl::Introspection::ubos, {{"N", instances}});
    vup::Compute_shader rotate_box("../../src/shader/compute/rotate_box.comp");
    bounds_vao.get_vbo(0).bind_base(4);
    sph.update_uniform("dt", delta);
    bb_model = glm::rotate(bb_model, delta * 0.1f, glm::vec3(0.3, 0.3f, 0.3f));
    rotate_box.update_uniform("model", bb_model);
    auto loop = [&](float dt) {
        rotate_box.run(bounds_cube.vertices.size());
        cam.update(window, dt);
        mats.update(model, cam.get_view(), cam.get_projection());
        sph.run(instances);
        particle_shader.update_ubo("mvp", mats);
        particle_shader.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        particle_spheres.render(GL_TRIANGLES,
                                static_cast<unsigned int>(instances));
        box_renderer.use();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        bounds_vao.render(GL_TRIANGLES);
        gl_debug_logger.retrieve_log(std::cout);
    };
    window.run_loop_fixed(delta, loop);
    glfwTerminate();
    return 0;
}