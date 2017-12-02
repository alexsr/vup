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
        inline void set_clear_color(float r, float g, float b, float a) {
            glClearColor(r, g, b, a);
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
        constexpr std::underlying_type_t<Storage> to_gl(Storage s) {
            return static_cast<std::underlying_type_t<Storage>>(s);
        }
        constexpr Storage operator|(Storage s1, Storage s2) {
            return static_cast<Storage>(to_gl(s1) | to_gl(s2));
        }
        constexpr bool operator&(Storage s1, Storage s2) {
            return (to_gl(s1) & to_gl(s2)) != 0;
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
        constexpr std::underlying_type_t<Access> to_gl(Access s) {
            return static_cast<std::underlying_type_t<Access>>(s);
        }
        constexpr Access operator|(Access s1, Access s2) {
            return static_cast<Access>(to_gl(s1) | to_gl(s2));
        }
        constexpr bool operator&(Access s1, Access s2) {
            return (to_gl(s1) & to_gl(s2)) != 0;
        }
        enum class Introspection : GLbitfield {
            none = 0, basic = 2, ubos = 4, ssbos = 8
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
    namespace tex {
        enum class Format : GLenum {
            r = GL_RED, b = GL_BLUE, g = GL_GREEN, rg = GL_RG,
            rbg = GL_RGB, bgr = GL_BGR, rgba = GL_RGBA, bgra = GL_BGRA,
            r_int = GL_RED_INTEGER, g_int = GL_GREEN_INTEGER,
            b_int = GL_BLUE_INTEGER, rg_int = GL_RG_INTEGER,
            rgb_int = GL_RGB_INTEGER, bgr_int = GL_BGR_INTEGER,
            rgba_int = GL_RGBA_INTEGER, bgra_int = GL_BGRA_INTEGER,
            depth = GL_DEPTH_COMPONENT, stencil = GL_STENCIL_INDEX,
            depth_stencil = GL_DEPTH_STENCIL
        };
        constexpr std::underlying_type_t<Format> to_gl(Format i) {
            return static_cast<std::underlying_type_t<Format>>(i);
        }
        enum class Type : GLenum {
            ubyte = GL_UNSIGNED_BYTE, byte = GL_BYTE, ushort = GL_UNSIGNED_SHORT,
            s = GL_UNSIGNED_SHORT, uint = GL_UNSIGNED_INT, i = GL_INT, f = GL_FLOAT
        };
        constexpr std::underlying_type_t<Type> to_gl(Type i) {
            return static_cast<std::underlying_type_t<Type>>(i);
        }
    }
    struct FBO_tex_desc {
        GLenum target = GL_TEXTURE_2D;
        tex::Format format = tex::Format::rgba;
        GLint sized_format = GL_RGBA;
        tex::Type type = tex::Type::ubyte;
        bool as_rbo = false;
    };
}

#endif //VUP_GL_UTILS_H
