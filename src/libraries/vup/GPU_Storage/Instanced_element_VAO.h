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
    class Instanced_element_VAO : public vup::Instanced_VAO {
    public:
        explicit Instanced_element_VAO(const vup::VBO& main_vbo,
                                       const vup::Element_buffer& element_vbo,
                                       const std::initializer_list<vup::Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_element_VAO(const vup::VBO& main_vbo,
                                       const vup::Element_buffer& element_vbo,
                                       const std::initializer_list<vup::VBO>& vbos,
                                       const std::initializer_list<vup::Instanced_VBO>& instanced_vbos);
        explicit Instanced_element_VAO(const vup::Geometric_primitive& primitive,
                                       const std::initializer_list<vup::Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_element_VAO(const vup::Geometric_primitive& primitive,
                                       const std::initializer_list<vup::VBO>& vbos,
                                       const std::initializer_list<vup::Instanced_VBO>& instanced_vbos);
        void render(GLenum render_mode, unsigned int instances) override;
        void render(GLenum render_mode, int offset, unsigned int count,
                    unsigned int instances) override;
    };
}

#endif //VUP_INSTANCED_ELEMENT_VAO_H
