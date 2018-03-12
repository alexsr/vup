//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Instanced_VAO.h"

vup::Instanced_VAO::Instanced_VAO(const VBO& main_vbo,
                                  const std::initializer_list<Instanced_VBO>& instanced_vbos)
    : Base_VAO(main_vbo, {}) {
    unsigned int i = 1;
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const VBO& main_vbo,
                                  const std::initializer_list<VBO>& vbos,
                                  const std::initializer_list<Instanced_VBO>& instanced_vbos)
    : Base_VAO(main_vbo, vbos) {
    auto i = static_cast<unsigned int>(vbos.size() + 1);
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const Geometric_primitive& primitive,
                                  const std::initializer_list<Instanced_VBO>& instanced_vbos)
    : Base_VAO(primitive) {
    unsigned int i = 3;
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const Geometric_primitive& primitive,
                                  const std::initializer_list<VBO>& vbos,
                                  const std::initializer_list<Instanced_VBO>& instanced_vbos)
    : Base_VAO(primitive, vbos) {
    auto i = static_cast<unsigned int>(vbos.size() + 3);
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const Mesh& mesh,
                                  const std::initializer_list<Instanced_VBO>& instanced_vbos)
    : Base_VAO(mesh) {
    auto i = static_cast<unsigned int>(mesh.get_vbos().size());
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

void vup::Instanced_VAO::set_divisor_qualifier(const Instanced_VBO& v,
                                               const unsigned int index) const {
    glVertexArrayBindingDivisor(m_name, index, v.get_divisor());
}

void vup::Instanced_VAO::render(const GLenum render_mode, const unsigned int instances) const {
    bind();
    glDrawArraysInstanced(render_mode, 0, m_count, instances);
}

void vup::Instanced_VAO::render(const GLenum render_mode, const int offset, const unsigned int count,
                                const unsigned int instances) const {
    bind();
    glDrawArraysInstanced(render_mode, offset, count, instances);
}
