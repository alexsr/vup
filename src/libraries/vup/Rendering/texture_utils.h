//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_TEXTURE_UTILS_H
#define VUP_TEXTURE_UTILS_H

#include <vup/Core/vup.h>

namespace vup
{
    namespace gl
    {
        enum class Tex_format : GLenum {
            r = GL_RED, b = GL_BLUE, g = GL_GREEN, rg = GL_RG,
            rbg = GL_RGB, bgr = GL_BGR, rgba = GL_RGBA, bgra = GL_BGRA,
            r_int = GL_RED_INTEGER, g_int = GL_GREEN_INTEGER,
            b_int = GL_BLUE_INTEGER, rg_int = GL_RG_INTEGER,
            rgb_int = GL_RGB_INTEGER, bgr_int = GL_BGR_INTEGER,
            rgba_int = GL_RGBA_INTEGER, bgra_int = GL_BGRA_INTEGER,
            depth = GL_DEPTH_COMPONENT, stencil = GL_STENCIL_INDEX,
            depth_stencil = GL_DEPTH_STENCIL
        };
        constexpr std::underlying_type_t <Tex_format> to_gl(Tex_format i) {
            return static_cast<std::underlying_type_t <Tex_format>>(i);
        }
        enum class Tex_type : GLenum {
            ubyte = GL_UNSIGNED_BYTE,
            byte = GL_BYTE,
            ushort = GL_UNSIGNED_SHORT,
            s = GL_UNSIGNED_SHORT,
            uint = GL_UNSIGNED_INT,
            i = GL_INT,
            f = GL_FLOAT
        };
        constexpr std::underlying_type_t <Tex_type> to_gl(Tex_type i) {
            return static_cast<std::underlying_type_t <Tex_type>>(i);
        }
        enum class Img_access : GLenum {
            read = GL_READ_ONLY, write = GL_WRITE_ONLY, read_write = GL_READ_WRITE
        };
        constexpr std::underlying_type_t <Img_access> to_gl(Img_access i) {
            return static_cast<std::underlying_type_t <Img_access>>(i);
        }
    }
    struct FBO_attachment {
        GLenum target = GL_TEXTURE_2D;
        gl::Tex_format format = gl::Tex_format::rgba;
        GLint sized_format = GL_RGBA;
        gl::Tex_type type = gl::Tex_type::ubyte;
        bool as_rbo = false;
    };
}

#endif //VUP_TEXTURE_UTILS_H
