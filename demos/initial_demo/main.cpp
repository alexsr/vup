#include "vup/Core/utils.h"

int main() {
    GLFWwindow* window = vup::create_window(800,600,"Initial Testing", nullptr, nullptr);
    vup::init_GLEW();
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}