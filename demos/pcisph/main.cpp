#include "vup/Core/utils.h"

int main() {
    GLFWwindow* window = vup::createWindow(800,600,"PCISPH", 0, 0);
    vup::initGLEW();
    while(!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}