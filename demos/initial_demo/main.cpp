//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "vup/Core/demo_utils.h"
#include "vup/Rendering/Trackball_camera.h"
#include <memory>

auto cam = std::make_unique<vup::Trackball_camera>(800, 600);

void resize_callback(GLFWwindow* w, int width, int height) {
    cam->resize(width, height);
    glViewport(0, 0, width, height);
}

int main() {
    GLFWwindow* window = vup::create_window(800, 600, "Initial Testing",
                                            nullptr, nullptr);
    vup::init_GLEW();
    glfwSetWindowSizeCallback(window, resize_callback);
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}