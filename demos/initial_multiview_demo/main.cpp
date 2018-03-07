//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Shader/V_F_shader.h>
#include <vup/Utility/OpenGL_debug_logger.h>
#include <vup/GPU_Storage/Instanced_VAO.h>
#include <vup/Geometry/Mesh_loader.h>

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
    int height = 800;
    vup::Window window(width, height, "Initial instanced rendering demo", true);
    vup::Trackball_camera cam(width, height);
    vup::print_context_info();
    vup::init_demo_OpenGL_params();
    unsigned int instances = 16 * 16;
    vup::V_F_shader multiviewport("../../src/shader/multiview/multiview.vert",
                                  "../../src/shader/multiview/multiview.frag",
                                  vup::gl::introspection::basic,
                                  {{"N", std::to_string(instances)}});
    vup::Mesh_loader bunny_loader("../../resources/meshes/bunny.obj");
    vup::Mesh bunny(bunny_loader.get_mesh_data(0));
    vup::Cube c;
    auto color_vec = vup::generate_random_float_data(instances * 4, 0, 1);
    vup::Instanced_VBO color(color_vec);
    vup::Instanced_VAO vao(bunny, {color});
    vup::OpenGL_debug_logger gl_debug_logger;
    gl_debug_logger.disable_messages(GL_DONT_CARE, GL_DONT_CARE,
                                     GL_DEBUG_SEVERITY_NOTIFICATION);
    std::vector<MVP> mats(instances);
    vup::UBO mats_ubo(mats, 9);
    auto resize_callback = [](GLFWwindow* window, int w, int h) {
        glViewport(0, 0, w, h);
    };
    resize_callback(nullptr, width, height);
    window.set_resize(resize_callback);
    auto loop = [&](float dt) {
        multiviewport.use();
        cam.update(window, dt);
        unsigned int viewcount_x = 16;
        unsigned int viewcount_y = instances / viewcount_x;
        float right = viewcount_x / 2.0f;
        float top = viewcount_y / 2.0f;
        for (int x = 0; x < viewcount_x; x++) {
            for (int y = 0; y < viewcount_y; y++) {
                unsigned long i = x * viewcount_y + y;
                float px_step_x = width / viewcount_x;
                float px_step_y = height / viewcount_y;
                glm::vec3 obj_pos(x + 0.5 - right, -y + top - 0.5, 0);
                glm::mat4 model(1.0f);
                glm::mat4 view = glm::lookAt(obj_pos + glm::vec3(0, 0, 1),
                                             obj_pos, glm::vec3(0, 1, 0));
                glm::mat4 ortho = glm::ortho(-right, right, -top, top, 0.5f, 1000.0f);
                mats.at(i).update(model, view, ortho);
            }
        }
        mats_ubo.update_data(mats);
        vao.render(GL_TRIANGLES, instances);
        gl_debug_logger.retrieve_log(std::cout);
    };
    window.run_loop_fixed(0.01f, loop);
    glfwTerminate();
    return 0;
}
