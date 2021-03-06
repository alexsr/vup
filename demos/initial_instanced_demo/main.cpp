//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/demo_utils.h>
#include <vup/Rendering/Trackball_camera.h>
#include <vup/Shader/Shader.h>
#include <vup/GPU_Storage/VAO.h>

int main() {
    vup::Window window(800, 600, "Initial instanced rendering demo");
    vup::Trackball_camera cam(800, 600);
    vup::init_demo_OpenGL_params();
    vup::V_F_shader minimal("../../src/shader/rendering/mvp_instanced.vert",
                            "../../src/shader/rendering/minimal_instanced.frag");
    unsigned int instances = 10;
    auto offset = std::make_shared<vup::Instanced_VBO>(vup::generate_random_float_data(instances * 2, -1.0f, 1.0f), 2);
    auto color = std::make_shared<vup::Instanced_VBO>(vup::generate_random_float_data(instances * 3, 0.0f, 1.0f), 3);
    vup::Instanced_VAO vao(vup::Cube(1.0f), {offset, color});
    bool allow_reset = true;
    minimal.update_uniform("model", glm::mat4(1.0f));
    while (!window.should_close()) {
        vup::gl::clear_buffers();
        minimal.use();
        cam.update(window, 0.01f);
        minimal.update_uniform("view", cam.get_view());
        minimal.update_uniform("proj", cam.get_projection());
        vao.render(GL_TRIANGLES, instances);
        if (glfwGetKey(window.get(), GLFW_KEY_X) == GLFW_PRESS && allow_reset) {
            offset->update_data(vup::generate_random_float_data(instances * 2, -1.0f, 1.0f));
            color->update_data(vup::generate_random_float_data(instances * 3, 0.0f, 1.0f));
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
