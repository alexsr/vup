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

        enum class Introspection : GLbitfield {
            none = 0,
            basic = 2,
            ubos = 4,
            ssbos = 8
        };

        constexpr std::underlying_type_t<Introspection> to_gl(Introspection i) {
            return static_cast<std::underlying_type_t<Introspection>>(i);
        }

        constexpr Introspection operator|(Introspection i1, Introspection i2) {
            return static_cast<Introspection>(to_gl(i1) | to_gl(i2));
        }

        constexpr bool operator&(Introspection i1, Introspection i2) {
            return (to_gl(i1) & to_gl(i2)) != 0;
        }
    }
}

#endif //VUP_SHADER_UTILS_H
