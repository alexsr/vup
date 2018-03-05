//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_TEXTURE_H
#define VUP_TEXTURE_H

#include "texture_utils.h"

namespace vup
{
    class Texture {
    public:
        Texture(FBO_attachment t, int width, int height);
        void resize(int width, int height);
        GLuint get_id();
        GLenum get_target();
        void activate(GLuint unit);
        void bind_to_image(GLuint unit, gl::Img_access access = gl::Img_access::read_write,
                           GLenum format = GL_RGBA8);
    private:
        int m_width = 0;
        int m_height = 0;
        GLenum m_target = GL_TEXTURE_2D;
        gl::Tex_format m_format = gl::Tex_format::rgba;
        GLint m_sized_format = GL_RGBA;
        gl::Tex_type m_type = gl::Tex_type::ubyte;
        GLuint m_id = 0;
        GLenum m_min_filter;
        GLenum m_mag_filter;
    };
}

#endif //VUP_TEXTURE_H
