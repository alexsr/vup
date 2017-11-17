//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "vup/Core/demo_utils.h"
#include "vup/Rendering/Trackball_camera.h"
#include <memory>
#include <vup/GPU_Storage/VAO.h>
#include <vup/Core/Window.h>
#include "vup/Rendering/V_F_shader_program.h"

int main() {
    vup::init_GLFW();
    vup::Window window(800, 600, "Initial instanced rendering demo");
    vup::Trackball_camera cam(800, 600);
    vup::init_GLEW();
    vup::init_demo_OpenGL_params();
    glEnable(GL_POINT_SPRITE);
    auto minimal_vertex(std::make_shared<vup::Vertex_shader>("../../src/shader/point.vert"));
    auto minimal_fragment(std::make_shared<vup::Fragment_shader>("../../src/shader/point.frag"));
    vup::V_F_shader_program minimal(minimal_vertex, minimal_fragment);
    unsigned int instances = 100;
    glPointSize(105.0f);
    vup::VBO positions(vup::generate_random_data(instances * 3, -1, 1), 3);
    vup::VBO color(vup::generate_random_data(instances * 3, 0, 1), 3);
    vup::VAO vao(positions, {color});
    bool allow_reset = true;
    while (window.should_close()) {
        vup::clear_buffers();
        minimal.use();
        cam.update(window.get_GLFWwindow(), 0.01f);
        minimal.update_uniform("view", cam.get_view());
        minimal.update_uniform("proj", cam.get_projection());
        vao.render(GL_POINTS);
        if (glfwGetKey(window.get(), GLFW_KEY_X) == GLFW_PRESS && allow_reset) {
            positions.update_data(vup::generate_random_data(instances * 3, -1, 1));
            color.update_data(vup::generate_random_data(instances * 3, 0, 1));
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