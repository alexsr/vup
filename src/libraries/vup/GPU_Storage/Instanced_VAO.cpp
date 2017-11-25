//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Instanced_VAO.h"

vup::Instanced_VAO::Instanced_VAO(const vup::VBO& main_vbo,
                                  const std::initializer_list<vup::Instanced_VBO>& instanced_vbos)
        : Base_VAO(main_vbo, {}) {
    unsigned int i = 1;
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const vup::VBO& main_vbo,
                                  const std::initializer_list<vup::VBO>& vbos,
                                  const std::initializer_list<vup::Instanced_VBO>& instanced_vbos)
        : Base_VAO(main_vbo, vbos) {
    auto i = static_cast<unsigned int>(vbos.size() + 1);
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const vup::Geometric_primitive& primitive,
                                  const std::initializer_list<vup::Instanced_VBO>& instanced_vbos)
        : Base_VAO(primitive) {
    unsigned int i = 3;
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const vup::Geometric_primitive& primitive,
                                  const std::initializer_list<vup::VBO>& vbos,
                                  const std::initializer_list<vup::Instanced_VBO>& instanced_vbos)
        : Base_VAO(primitive, vbos) {
    auto i = static_cast<unsigned int>(vbos.size() + 3);
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const vup::Mesh& mesh,
                                  const std::initializer_list<vup::Instanced_VBO>& instanced_vbos)
        : Base_VAO(mesh) {
    auto i = static_cast<unsigned int>(mesh.get_VBOs().size());
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

void vup::Instanced_VAO::set_divisor_qualifier(const vup::Instanced_VBO& v,
                                               unsigned int index) {
    glVertexArrayBindingDivisor(m_name, index, v.get_divisor());
}

void vup::Instanced_VAO::render(GLenum render_mode, unsigned int instances) {
    bind();
    glDrawArraysInstanced(render_mode, 0, m_count, instances);
}

void vup::Instanced_VAO::render(GLenum render_mode, int offset, unsigned int count,
                                unsigned int instances) {
    bind();
    glDrawArraysInstanced(render_mode, offset, count, instances);
}
