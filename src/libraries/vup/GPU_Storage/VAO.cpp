//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "VAO.h"

vup::VAO::VAO(const VBO& main_vbo,
              const std::initializer_list<VBO>& vbos)
    : Base_VAO(main_vbo, vbos) {
}

vup::VAO::VAO(const Geometric_primitive& primitive,
              const std::initializer_list<VBO>& vbos)
    : Base_VAO(primitive, vbos) {
}

vup::VAO::VAO(const Mesh& mesh, const std::initializer_list<VBO>& vbos) : Base_VAO(mesh, vbos) {
}

void vup::VAO::render(const GLenum render_mode) const {
    bind();
    glDrawArrays(render_mode, 0, m_count);
}

void vup::VAO::render(const GLenum render_mode, const int offset, const unsigned int count) const {
    bind();
    glDrawArrays(render_mode, offset, count);
}
