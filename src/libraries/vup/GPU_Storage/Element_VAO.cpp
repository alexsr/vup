//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Element_VAO.h"

vup::Element_VAO::Element_VAO(const VBO& main_vbo,
                              const Element_buffer& element_vbo,
                              const std::initializer_list<VBO>& vbos)
    : VAO(main_vbo, vbos) {
    glVertexArrayElementBuffer(m_name, element_vbo.get_name());
    m_count = element_vbo.get_count();
}

vup::Element_VAO::Element_VAO(const Geometric_primitive& primitive,
                              const std::initializer_list<VBO>& vbos)
    : VAO(primitive, vbos) {
    Element_buffer element_vbo(primitive.indices);
    glVertexArrayElementBuffer(m_name, element_vbo.get_name());
    m_count = element_vbo.get_count();
}

vup::Element_VAO::Element_VAO(Mesh mesh) : VAO(mesh) {
    glVertexArrayElementBuffer(m_name, mesh.get_index_buffer().get_name());
    m_count = mesh.get_index_buffer().get_count();
}

void vup::Element_VAO::render(const GLenum render_mode) const {
    bind();
    glDrawElements(render_mode, m_count, GL_UNSIGNED_INT, nullptr);
}

void vup::Element_VAO::render(const GLenum render_mode, const int offset, const unsigned int count) const {
    bind();
    glDrawElements(render_mode, count, GL_UNSIGNED_INT, &offset);
}
