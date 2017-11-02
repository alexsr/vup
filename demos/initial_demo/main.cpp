//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "vup/Core/demo_utils.h"
#include "vup/Rendering/Trackball_camera.h"
#include <memory>
#include <vup/Core/Window.h>
#include "vup/Rendering/V_F_shader_program.h"
#include "vup/Rendering/Geometric_primitives.h"
#include "vup/GPU_Storage/VAO.h"

int main() {
    vup::init_GLFW();
    vup::Window window(800, 600, "Initial instanced rendering demo");
    vup::Trackball_camera cam(800, 600);
    vup::init_GLEW();
    vup::set_viewport(800, 600);
    vup::init_demo_OpenGL_params();
    auto minimal_vertex(std::make_shared<vup::Vertex_shader>("../../src/shader/minimal.vert"));
    auto minimal_fragment(std::make_shared<vup::Fragment_shader>("../../src/shader/minimal.frag"));
    vup::V_F_shader_program minimal(minimal_vertex, minimal_fragment);
    vup::VAO vao(vup::VBO(vup::Quad().vertices));
    while (window.should_close()) {
        vup::clear_buffers();
        minimal.use();
        cam.update(window.get_GLFWwindow(), 0.01f);
        minimal.update_uniform("view", cam.get_view());
        minimal.update_uniform("proj", cam.get_projection());
        vao.render(GL_TRIANGLE_STRIP);
        window.swap_buffer();
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}