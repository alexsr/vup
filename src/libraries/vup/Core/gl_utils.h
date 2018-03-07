//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_GL_UTILS_H
#define VUP_GL_UTILS_H

#include "vup.h"
#include <stdexcept>
#include <string>
#include <array>

namespace vup
{
    namespace gl
    {
        inline void init_gl() {
            if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
                throw std::runtime_error{"Failed to initialize OpenGL."};
            }
        }

        inline void clear_buffers() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        inline void set_clear_color(const float r, const float g, const float b, const float a) {
            glClearColor(r, g, b, a);
        }

        inline void set_viewport(const int width, const int height) {
            glViewport(0, 0, width, height);
        }

        inline void set_viewport(const int x, const int y, const int width, const int height) {
            glViewport(x, y, width, height);
        }
    }
}

#endif //VUP_GL_UTILS_H
