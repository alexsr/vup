//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Rendering/V_F_shader_program.h>
#include <vup/GPU_Storage/VAO.h>

int main() {
    vup::init_GLFW();
    vup::Window window(800, 600, "Initial instanced rendering demo");
    vup::Trackball_camera cam(800, 600);
    vup::init_GLEW();
    vup::init_demo_OpenGL_params();
    auto minimal_vertex(std::make_shared<vup::Vertex_shader>("../../src/shader/minimal.vert"));
    auto minimal_fragment(std::make_shared<vup::Fragment_shader>("../../src/shader/minimal.frag"));
    vup::V_F_shader_program minimal(minimal_vertex, minimal_fragment);
    vup::Quad q;
    vup::VBO vertices(q.vertices);
    vup::VBO normals(q.normals, 3);
    vup::VBO uv_coords(q.uv_coords, 2);
    vup::VAO vao(vertices, {normals, uv_coords});
    while (!window.should_close()) {
        vup::clear_buffers();
        minimal.use();
        vao.render(GL_TRIANGLE_STRIP);
        window.swap_buffer();
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}