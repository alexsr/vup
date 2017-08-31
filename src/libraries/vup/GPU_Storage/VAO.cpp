//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "VAO.h"

vup::VAO::VAO(const vup::VBO& main_vbo,
              const std::initializer_list<vup::VBO>& vbos)
: Base_VAO(main_vbo, vbos) {
}

void vup::VAO::render(GLenum render_mode) {
    bind();
    glDrawArrays(render_mode, 0, m_count);
}

void vup::VAO::render(GLenum render_mode, int offset, unsigned int count) {
    bind();
    glDrawArrays(render_mode, offset, count);
}
