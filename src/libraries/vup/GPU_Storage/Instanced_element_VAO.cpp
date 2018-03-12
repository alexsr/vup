//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Instanced_element_VAO.h"

vup::Instanced_element_VAO::Instanced_element_VAO(const VBO& main_vbo,
                                                  const Element_buffer& element_vbo,
                                                  const std::initializer_list<Instanced_VBO>& instanced_vbos)
    : Instanced_VAO(main_vbo, instanced_vbos) {
    glVertexArrayElementBuffer(m_name, element_vbo.get_name());
    m_count = element_vbo.get_count();
}

vup::Instanced_element_VAO::Instanced_element_VAO(const VBO& main_vbo,
                                                  const Element_buffer& element_vbo,
                                                  const std::initializer_list<VBO>& vbos,
                                                  const std::initializer_list<Instanced_VBO>& instanced_vbos)
    : Instanced_VAO(main_vbo, vbos, instanced_vbos) {
    glVertexArrayElementBuffer(m_name, element_vbo.get_name());
    m_count = element_vbo.get_count();
}

vup::Instanced_element_VAO::Instanced_element_VAO(const Geometric_primitive& primitive,
                                                  const std::initializer_list<Instanced_VBO>& instanced_vbos)
    : Instanced_VAO(primitive, instanced_vbos) {
    Element_buffer element_vbo(primitive.indices);
    glVertexArrayElementBuffer(m_name, element_vbo.get_name());
    m_count = element_vbo.get_count();
}

vup::Instanced_element_VAO::Instanced_element_VAO(const Geometric_primitive& primitive,
                                                  const std::initializer_list<VBO>& vbos,
                                                  const std::initializer_list<Instanced_VBO>& instanced_vbos)
    : Instanced_VAO(primitive, vbos, instanced_vbos) {
    Element_buffer element_vbo(primitive.indices);
    glVertexArrayElementBuffer(m_name, element_vbo.get_name());
    m_count = element_vbo.get_count();
}

void vup::Instanced_element_VAO::render(const GLenum render_mode, const unsigned int instances) const {
    bind();
    glDrawElementsInstanced(render_mode, m_count, GL_UNSIGNED_INT, nullptr, instances);
}

void
vup::Instanced_element_VAO::render(const GLenum render_mode, const int offset, const unsigned int count,
                                   const unsigned int instances) const {
    bind();
    glDrawElementsInstanced(render_mode, count, GL_UNSIGNED_INT, &offset, instances);
}
