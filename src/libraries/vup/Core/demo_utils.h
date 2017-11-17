//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_DEMO_UTILS_H
#define VUP_DEMO_UTILS_H

#include "vup.h"
#include "Window.h"
#include <iostream>
#include <random>
#include <functional>

namespace vup
{
    inline void init_GLFW() {
        int glfw_error = glfwInit();
        if (glfw_error == 0) {
            throw std::runtime_error{"Failed to initialize GLFW."};
        }
    }
    inline void print_context_info() {
        GLint major;
        glGetIntegerv(GL_MAJOR_VERSION, &major);
        GLint minor;
        glGetIntegerv(GL_MINOR_VERSION, &minor);
        std::cout << "OpenGL Version: " << major << "." << minor << "\n";
        std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
        std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
        std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    }
    inline void init_demo_OpenGL_params() {
        glEnable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
