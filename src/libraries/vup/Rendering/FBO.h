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
#include <string>

namespace vup
{
    class FBO {
    public:
        FBO(int width, int height, std::vector<Texture_definition> textures);
        void bind();
        void unbind();
        Texture get_texture(unsigned long i);
    private:
        void attach_color_component(Texture_definition t, unsigned int i);
        void attach_depth_component(Texture_definition t);
        void check_fbo_status();
        GLuint m_id = 0;
        int m_width;
        int m_height;
        std::vector<Texture> m_textures;
        std::vector<Renderbuffer> m_renderbuffers;
    };
}

#endif //VUP_FBO_H
