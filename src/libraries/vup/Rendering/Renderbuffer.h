//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_RENDERBUFFER_H
#define VUP_RENDERBUFFER_H

#include <vup/Core/gl_utils.h>

namespace vup
{
    class Renderbuffer {
    public:
        Renderbuffer(FBO_tex_desc t, int width, int height);
        void resize(int width, int height);
        GLuint get_id();
    private:
        int m_width = 0;
        int m_height = 0;
        GLenum m_target = GL_TEXTURE_2D;
        tex::Format m_format = tex::Format::rgba;
        GLint m_sized_format = GL_RGBA;
        tex::Type m_type = tex::Type::ubyte;
        GLuint m_id = 0;
    };
}

#endif //VUP_RENDERTARGET_H
