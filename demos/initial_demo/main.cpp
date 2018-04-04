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
    vup::Window window(800, 600, "Initial instanced rendering demo");
    vup::Trackball_camera cam(800, 600);
    vup::init_demo_OpenGL_params();
    vup::V_F_shader minimal("../../src/shader/rendering/minimal.vert", "../../src/shader/rendering/minimal.frag");
    auto resize_callback = [](GLFWwindow* window, int w, int h) { glViewport(0, 0, w, h); };
    resize_callback(nullptr, 800, 600);
    window.set_resize(resize_callback);
    vup::Cube q(1.0f);
    vup::VBO vertices(q.vertices);
    vup::VBO normals(q.normals, 3);
    vup::VBO uv_coords(q.uv_coords, 2);
    vup::VAO vao(vertices, {normals, uv_coords});
    while (!window.should_close()) {
        vup::gl::clear_buffers();
        minimal.use();
        vao.render(GL_TRIANGLE_STRIP);
        window.swap_buffer();
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
