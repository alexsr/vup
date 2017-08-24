//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_DEMO_UTILS_H
#define VUP_DEMO_UTILS_H

#include "vup.h"
#include <stdexcept>
#include <iostream>

namespace vup
{
    GLFWwindow* create_window(int width, int height, const char* title,
                              GLFWmonitor* monitor, GLFWwindow* share) {
        int glfw_error = glfwInit();
        if (glfw_error == 0) {
            throw std::runtime_error{"Failed to initialize GLFW."};
        }
        auto window = glfwCreateWindow(width, height, title, monitor, share);
        if (window == nullptr) {
            glfwTerminate();
            throw std::runtime_error{"Failed to create GLFWwindow."};
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);
        return window;
    }

    void init_GLEW() {
        GLenum glew_error = glewInit();
        if (GLEW_OK != glew_error) {
            throw std::runtime_error{"Failed to initialize GLEW."};
        }
    }
}

#endif //VUP_DEMO_UTILS_H
