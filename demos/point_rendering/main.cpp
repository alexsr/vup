//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Shader/V_F_shader.h>
#include <vup/GPU_Storage/VAO.h>

int main() {
    vup::init_GLFW();
    vup::Window window(800, 600, "Initial instanced rendering demo");
    vup::Trackball_camera cam(800, 600);
    vup::init_demo_OpenGL_params();
    glEnable(GL_POINT_SPRITE);
    vup::V_F_shader minimal("../../src/shader/point.vert", "../../src/shader/point.frag");
    auto resize_callback = [](GLFWwindow* window, int w, int h) { glViewport(0, 0, w, h); };
    resize_callback(nullptr, 800, 600);
    window.set_resize(resize_callback);
    unsigned int instances = 100;
    glPointSize(105.0f);
    vup::VBO positions(vup::generate_random_float_data(instances * 3, -1, 1), 3);
    vup::VBO color(vup::generate_random_float_data(instances * 3, 0, 1), 3);
    vup::VAO vao(positions, {color});
    bool allow_reset = true;
    while (!window.should_close()) {
        vup::gl::clear_buffers();
        minimal.use();
        cam.update(window, 0.01f);
        minimal.update_uniform("view", cam.get_view());
        minimal.update_uniform("proj", cam.get_projection());
        vao.render(GL_POINTS);
        if (glfwGetKey(window.get(), GLFW_KEY_X) == GLFW_PRESS && allow_reset) {
            positions.update_data(vup::generate_random_float_data(instances * 3, -1, 1));
            color.update_data(vup::generate_random_float_data(instances * 3, 0, 1));
            allow_reset = false;
        }
        if (glfwGetKey(window.get(), GLFW_KEY_X) == GLFW_RELEASE) {
            allow_reset = true;
        }
        window.swap_buffer();
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}