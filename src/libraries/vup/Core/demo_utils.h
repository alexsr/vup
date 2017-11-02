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
    inline void init_GLFW() {
        int glfw_error = glfwInit();
        if (glfw_error == 0) {
            throw std::runtime_error{"Failed to initialize GLFW."};
        }
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
