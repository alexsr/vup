//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_FBO_H
#define VUP_FBO_H

#include "Texture.h"
#include "Renderbuffer.h"
#include <vector>
#include <stdexcept>
#include <map>
#include <string>

namespace vup
{
    class FBO {
    public:
        FBO(int width, int height, std::vector<vup::FBO_attachment> textures);
        void bind();
        void unbind();
        vup::Texture get_texture(unsigned long i);
    private:
        void attach_color_component(FBO_attachment t, unsigned int i);
        void attach_depth_component(FBO_attachment t);
        void check_fbo_status();
        GLuint m_id = 0;
        int m_width;
        int m_height;
        std::vector<vup::Texture> m_textures;
        std::vector<vup::Renderbuffer> m_renderbuffers;
    };
}

#endif //VUP_FBO_H
