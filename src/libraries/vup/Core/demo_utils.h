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

namespace vup
{
    inline void init_demo_OpenGL_params() {
        glEnable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    inline std::vector<float> generate_random_float_data(int n, float lower_boundary,
                                                   float upper_boundary) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(lower_boundary, upper_boundary);
        std::vector<float> result(static_cast<unsigned long>(n));
        for (auto& r : result) {
            r = dis(gen);
        }
        return result;
    }

    struct MVP {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        void update(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) {
            model = m;
            view = v;
            projection = p;
        }
    };
}

#endif //VUP_DEMO_UTILS_H
