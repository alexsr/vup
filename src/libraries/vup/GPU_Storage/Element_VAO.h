//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_ELEMENT_VAO_H
#define VUP_ELEMENT_VAO_H

#include "VAO.h"
#include "Element_buffer.h"

namespace vup
{
    class Element_VAO : public VAO {
    public:
        explicit Element_VAO(const VBO& main_vbo, const Element_buffer& element_vbo,
                             const std::initializer_list<VBO>& vbos = {});
        explicit Element_VAO(const Geometric_primitive& primitive,
                             const std::initializer_list<VBO>& vbos = {});
        explicit Element_VAO(Mesh mesh);
        void render(GLenum render_mode) const override;
        void render(GLenum render_mode, int offset, unsigned int count) const override;
    };
}

#endif //VUP_ELEMENT_VAO_H
