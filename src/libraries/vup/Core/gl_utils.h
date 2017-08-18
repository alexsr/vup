//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_GL_UTILS_H
#define VUP_GL_UTILS_H

#include <GL/glew.h>
#include <string>

namespace vup
{
    std::string shader_type_to_string(GLenum type) {
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
