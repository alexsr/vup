//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "vup/Core/demo_utils.h"
#include "vup/Rendering/Trackball_camera.h"
#include <memory>
#include <vup/Rendering/V_F_shader_program.h>
#include <vup/Rendering/Geometry_primitives.h>
#include <vup/GPU_Storage/Instanced_VAO.h>

auto cam = std::make_unique<vup::Trackball_camera>(800, 600);

void resize_callback(GLFWwindow* w, int width, int height) {
    cam->resize(width, height);
    glViewport(0, 0, width, height);
}

int main() {
    auto window = vup::create_window(800, 600, "Initial Instanced Rendering Test", nullptr, nullptr);
    vup::init_GLEW();
    vup::set_viewport(800, 600);
    vup::init_demo_OpenGL_params();
    glfwSetWindowSizeCallback(window, resize_callback);
    auto minimal_vertex(std::make_shared<vup::Vertex_shader>("../../src/shader/minimal_instanced.vert"));
    auto minimal_fragment(std::make_shared<vup::Fragment_shader>("../../src/shader/minimal_instanced.frag"));
    vup::V_F_shader_program minimal(minimal_vertex, minimal_fragment);
    unsigned int instances = 10;
    vup::Instanced_VBO offset(vup::generate_random_data(instances * 2, -1, 1), 2);
    vup::Instanced_VBO color(vup::generate_random_data(instances * 3, 0, 1), 3);
    vup::Instanced_VAO vao(vup::VBO(vup::Quad().vertices), {offset, color});
    bool allow_reset = true;
    while (glfwWindowShouldClose(window) == 0) {
        vup::clear_buffers();
        minimal.use();
        vao.render(GL_TRIANGLE_STRIP, instances);
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && allow_reset) {
            offset.update_data(vup::generate_random_data(instances * 2, -1, 1));
            color.update_data(vup::generate_random_data(instances * 3, 0, 1));
            allow_reset = false;
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE) {
            allow_reset = true;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}