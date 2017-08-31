//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Element_VAO.h"

vup::Element_VAO::Element_VAO(const vup::VBO& main_vbo,
                              const vup::Element_buffer& element_vbo,
                              const std::initializer_list<vup::VBO>& vbos)
        : vup::VAO(main_vbo, vbos) {
    glVertexArrayElementBuffer(m_name, element_vbo.get_name());
    m_count = element_vbo.get_count();
}

void vup::Element_VAO::render(GLenum render_mode) {
    bind();
    glDrawElements(render_mode, m_count, GL_UNSIGNED_INT, nullptr);
}

void vup::Element_VAO::render(GLenum render_mode, int offset, int count) {
    bind();
    glDrawElements(render_mode, count, GL_UNSIGNED_INT, &offset);
}
