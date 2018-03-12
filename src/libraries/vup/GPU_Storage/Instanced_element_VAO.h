//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_INSTANCED_ELEMENT_VAO_H
#define VUP_INSTANCED_ELEMENT_VAO_H

#include "Instanced_VAO.h"
#include "Element_buffer.h"

namespace vup
{
    class Instanced_element_VAO : public Instanced_VAO {
    public:
        explicit Instanced_element_VAO(const VBO& main_vbo,
                                       const Element_buffer& element_vbo,
                                       const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_element_VAO(const VBO& main_vbo,
                                       const Element_buffer& element_vbo,
                                       const std::initializer_list<VBO>& vbos,
                                       const std::initializer_list<Instanced_VBO>& instanced_vbos);
        explicit Instanced_element_VAO(const Geometric_primitive& primitive,
                                       const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_element_VAO(const Geometric_primitive& primitive,
                                       const std::initializer_list<VBO>& vbos,
                                       const std::initializer_list<Instanced_VBO>& instanced_vbos);
        void render(GLenum render_mode, unsigned int instances) const override;
        void render(GLenum render_mode, int offset, unsigned int count,
                    unsigned int instances) const override;
    };
}

#endif //VUP_INSTANCED_ELEMENT_VAO_H
