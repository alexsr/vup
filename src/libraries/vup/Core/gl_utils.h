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
        enum class Storage : GLbitfield {
            dynamic = GL_DYNAMIC_STORAGE_BIT, read = GL_MAP_READ_BIT, write = GL_MAP_WRITE_BIT,
            persistent_read = GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT,
            persistent_write = GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT,
            coherent_read = GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT,
            coherent_write = GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT,
            client = GL_CLIENT_STORAGE_BIT

        };
        constexpr std::underlying_type_t<Storage> cast_to_bit(Storage s) {
            return static_cast<std::underlying_type_t<Storage>>(s);
        }
        constexpr Storage operator|(Storage s1, Storage s2) {
            return static_cast<Storage>(cast_to_bit(s1) | cast_to_bit(s2));
        }
        constexpr bool operator&(Storage s1, Storage s2) {
            return (cast_to_bit(s1) & cast_to_bit(s2)) != 0;
        }
        enum class Access : GLbitfield {
            read = GL_MAP_READ_BIT, write = GL_MAP_WRITE_BIT,
            persistent_read = GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT,
            persistent_write = GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT,
            coherent_read = GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT,
            coherent_write = GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT,
            invalidate_buffer = GL_MAP_INVALIDATE_BUFFER_BIT,
            invalidate_range = GL_MAP_INVALIDATE_RANGE_BIT, flush = GL_MAP_FLUSH_EXPLICIT_BIT,
            unsynchronized = GL_MAP_UNSYNCHRONIZED_BIT

        };
        constexpr std::underlying_type_t<Access> cast_to_bit(Access s) {
            return static_cast<std::underlying_type_t<Access>>(s);
        }
        constexpr Access operator|(Access s1, Access s2) {
            return static_cast<Access>(cast_to_bit(s1) | cast_to_bit(s2));
        }
        constexpr bool operator&(Access s1, Access s2) {
            return (cast_to_bit(s1) & cast_to_bit(s2)) != 0;
        }
        enum class Introspection : GLbitfield {
            none = 0, basic = 2, ubos = 4, ssbos = 8
        };
        constexpr std::underlying_type_t<Introspection> cast_to_bit(Introspection i) {
            return static_cast<std::underlying_type_t<Introspection>>(i);
        }
        constexpr Introspection operator|(Introspection i1, Introspection i2) {
            return static_cast<Introspection>(cast_to_bit(i1) | cast_to_bit(i2));
        }
        constexpr bool operator&(Introspection i1, Introspection i2) {
            return (cast_to_bit(i1) & cast_to_bit(i2)) != 0;
        }
    }
}

#endif //VUP_GL_UTILS_H
