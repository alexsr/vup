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

namespace vup
{
    namespace gl
    {
        inline void init_GLEW() {
            GLenum glew_error = glewInit();
            if (GLEW_OK != glew_error) {
                throw std::runtime_error{"Failed to initialize GLEW."};
            }
        }

        inline void clear_buffers() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        inline void set_clear_color(float r, float g, float b, float a) {
            glClearColor(r, g, b, a);
        }
    }
}

#endif //VUP_GL_UTILS_H
