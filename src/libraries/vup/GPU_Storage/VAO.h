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
    class VAO : public Base_VAO {
    public:
        explicit VAO(const VBO& main_vbo,
                     const std::initializer_list<VBO>& vbos = {});
        explicit VAO(const Geometric_primitive& primitive,
                     const std::initializer_list<VBO>& vbos = {});
        explicit VAO(const Mesh& mesh,
                     const std::initializer_list<VBO>& vbos = {});
        virtual void render(GLenum render_mode) const;
        virtual void render(GLenum render_mode, int offset, unsigned int count) const;
    };
}

#endif //VUP_VAO_H
