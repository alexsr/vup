//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Shader/V_F_shader.h>
#include <vup/Geometry/Mesh_loader.h>
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
    vup::Window window(width, height, "Randomly move bunny vertices", true);
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    vup::Trackball_camera cam(width, height);
    vup::print_context_info();
    vup::init_demo_OpenGL_params();
    vup::Compute_shader move_verts("../../src/shader/compute/randomly_move_verts.comp");
    vup::V_F_shader minimal("../../src/shader/mvp_ubo.vert", "../../src/shader/normal_rendering.frag",
                            vup::gl::introspection::ubos | vup::gl::introspection::ssbos);
    vup::Mesh_loader bunny_loader("../../resources/meshes/bunny.obj");
    vup::Mesh bunny(bunny_loader.get_mesh_data(0));
    vup::VAO vao(bunny);
    bunny.get_vbo(0).bind_base(0);
    auto resize_callback = [](GLFWwindow* window, int w, int h) { glViewport(0, 0, w, h); };
    resize_callback(nullptr, width, height);
    window.set_resize(resize_callback);
    vup::SSBO random_pos(vup::generate_random_float_data(bunny.get_count() * 4, 0, 1.0f), 4);
    glm::mat4 model(1.0f);
    MVP mats{model, cam.get_view(), cam.get_projection()};
    bool allow_reset;
    move_verts.update_uniform("dt", 0.0001f);
    auto loop = [&](float dt) {
        move_verts.run(bunny.get_count());
        minimal.use();
        cam.update(window, dt);
        mats.update(model, cam.get_view(), cam.get_projection());
        minimal.update_ubo("mvp", mats);
        vao.render(GL_TRIANGLES);
        gl_debug_logger.retrieve_log(std::cout);
        if (glfwGetKey(window.get(), GLFW_KEY_X) == GLFW_PRESS && allow_reset) {
            move_verts.reload();
            move_verts.update_uniform("dt", 0.0001f);
            random_pos.update_data(vup::generate_random_float_data(bunny.get_count() * 4, 0, 1.0f));
            allow_reset = false;
        }
        if (glfwGetKey(window.get(), GLFW_KEY_X) == GLFW_RELEASE) {
            allow_reset = true;
        }
    };
    window.run_loop_fixed(0.01f, loop);
    glfwTerminate();
    return 0;
}