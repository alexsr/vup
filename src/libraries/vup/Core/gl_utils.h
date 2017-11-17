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

namespace vup
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
    inline std::string shader_type_to_string(GLenum type) {
        switch (type) {
            case GL_VERTEX_SHADER:
                return "vertex shader";
            case GL_FRAGMENT_SHADER:
                return "fragment shader";
            case GL_GEOMETRY_SHADER:
                return "geometry shader";
            case GL_TESS_CONTROL_SHADER:
                return "tessellation control shader";
            case GL_TESS_EVALUATION_SHADER:
                return "tessellation evaluation shader";
            case GL_COMPUTE_SHADER:
                return "compute shader";
            default:
                return "unknown type of shader";
        }
    }
}

#endif //VUP_GL_UTILS_H
