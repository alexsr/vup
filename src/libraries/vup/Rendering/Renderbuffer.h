//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_RENDERBUFFER_H
#define VUP_RENDERBUFFER_H

#include "texture_utils.h"

namespace vup
{
    class Renderbuffer {
    public:
        Renderbuffer(Texture_definition t, int width, int height);
        void resize(int width, int height);
        GLuint get_id();
    private:
        int m_width = 0;
        int m_height = 0;
        GLenum m_target = GL_TEXTURE_2D;
        gl::Tex_format m_format = gl::Tex_format::rgba;
        GLint m_sized_format = GL_RGBA;
        gl::Tex_type m_type = gl::Tex_type::ubyte;
        GLuint m_id = 0;
    };
}

#endif //VUP_RENDERTARGET_H
