//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_SHADER_UTILS_H
#define VUP_SHADER_UTILS_H

#include <vup/Core/vup.h>
#include <string>

namespace vup
{
    namespace gl
    {
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

        enum class introspection : GLbitfield {
            none = 0,
            basic = 2,
            ubos = 4,
            ssbos = 8
        };

        constexpr std::underlying_type_t<introspection> to_gl(introspection i) {
            return static_cast<std::underlying_type_t<introspection>>(i);
        }

        constexpr introspection operator|(introspection i1, introspection i2) {
            return static_cast<introspection>(to_gl(i1) | to_gl(i2));
        }

        constexpr bool operator&(introspection i1, introspection i2) {
            return (to_gl(i1) & to_gl(i2)) != 0;
        }
    }
}

#endif //VUP_SHADER_UTILS_H
