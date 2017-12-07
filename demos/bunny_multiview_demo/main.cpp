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
#include <vup/GPU_Storage/Instanced_VAO.h>

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
    vup::Window window(width, height, "Initial instanced rendering demo", true);
    vup::Trackball_camera cam(width, height);
    vup::print_context_info();
    vup::init_demo_OpenGL_params();
    unsigned int instances = 16;
    vup::V_F_shader multiviewport("../../src/shader/multiview/multiview.vert",
                                  "../../src/shader/multiview/multiview.frag",
                                  vup::gl::Introspection::basic,
                                  {{"N", instances}});
    vup::Mesh_loader bunny_loader("../../resources/meshes/bunny.obj");
    vup::Mesh bunny(bunny_loader.get_mesh_data(0));
    auto color_vec = vup::generate_random_data(instances*4, 0, 1);
    vup::Instanced_VBO color(color_vec);
    vup::Instanced_VAO vao(bunny, {color});
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION);
    glm::mat4 model(1.0f);
    std::vector<MVP> mats(instances);
    vup::UBO mats_ubo(mats, 9);
    auto resize_callback = [](GLFWwindow* window, int w, int h) { glViewport(0, 0, w, h); };
    resize_callback(nullptr, width, height);
    window.set_resize(resize_callback);
    auto loop = [&](float dt) {
        multiviewport.use();
        cam.update(window, dt);
        for (auto&& m : mats) {
            model = glm::translate(model, glm::vec3(0, -0.2, 0));
            m.update(model, cam.get_view(), cam.get_projection());
        }
        model = glm::mat4(1.f);
        mats_ubo.update_data(mats);
        vao.render(GL_TRIANGLES, instances);
        gl_debug_logger.retrieve_log(std::cout);
    };
    window.run_loop_fixed(0.01f, loop);
    glfwTerminate();
    return 0;
}