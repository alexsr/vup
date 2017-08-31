//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_DEMO_UTILS_H
#define VUP_DEMO_UTILS_H

#include "vup/Core/vup.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <random>

namespace vup
{
    inline GLFWwindow* create_window(int width, int height, const char* title,
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

    inline void init_GLEW() {
        GLenum glew_error = glewInit();
        if (GLEW_OK != glew_error) {
            throw std::runtime_error{"Failed to initialize GLEW."};
        }
    }

    inline void set_viewport(int width, int height) {
        glViewport(0, 0, width, height);
    }

    inline void init_demo_OpenGL_params() {
        glEnable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    inline void clear_buffers() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    inline std::vector<float> generate_random_data(int n, float lower_boundary,
                                                   float upper_boundary) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(lower_boundary, upper_boundary);
        std::vector<float> result(static_cast<unsigned long>(n));
        for (auto& r: result) {
            r = dis(gen);
        }
        return result;
    }
}

#endif //VUP_DEMO_UTILS_H
