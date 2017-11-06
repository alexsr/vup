//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_VAO_H
#define VUP_VAO_H

#include "Base_VAO.h"

namespace vup
{
    class VAO : public vup::Base_VAO {
    public:
        explicit VAO(const vup::VBO& main_vbo,
                     const std::initializer_list<vup::VBO>& vbos = {});
        explicit VAO(const vup::Geometric_primitive& primitive,
                     const std::initializer_list<vup::VBO>& vbos = {});
        explicit VAO(vup::Mesh mesh);
        virtual void render(GLenum render_mode);
        virtual void render(GLenum render_mode, int offset, unsigned int count);
    };
}

#endif //VUP_VAO_H
