//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "vup/Core/demo_utils.h"
#include "vup/Rendering/Trackball_camera.h"
#include <vup/Core/Window.h>
#include <vup/GPU_Storage/Element_VAO.h>
#include <vup/Utility/OpenGL_debug_logger.h>
#include "vup/Rendering/V_F_shader_program.h"
#include "vup/Geometry/Mesh_loader.h"

int main() {
    vup::init_GLFW();
    vup::Window window(800, 600, "Initial instanced rendering demo", true);
    vup::init_GLEW();
    vup::Trackball_camera cam(800, 600);
    vup::print_context_info();
    vup::init_demo_OpenGL_params();
    auto minimal_vertex(std::make_shared<vup::Vertex_shader>("../../src/shader/mvp_minimal.vert"));
    auto minimal_fragment(std::make_shared<vup::Fragment_shader>("../../src/shader/uv_test.frag"));
    vup::V_F_shader_program minimal(minimal_vertex, minimal_fragment);
    vup::Mesh_loader bunny(RESOURCES_PATH "/meshes/bunny.obj");
    vup::VAO vao(bunny.get_mesh(0));
    vup::OpenGL_debug_logger gl_debug_logger;
    while (window.should_close()) {
        vup::clear_buffers();
        minimal.use();
        cam.update(window.get_GLFWwindow(), 0.01f);
        minimal.update_uniform("view", cam.get_view());
        minimal.update_uniform("proj", cam.get_projection());
        vao.render(GL_TRIANGLES);
        gl_debug_logger.retrieve_log(std::cout);
        window.swap_buffer();
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}