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

vup::VAO::VAO(const Mesh& mesh, const std::initializer_list<VBO>& vbos) : Base_VAO(mesh) {
}

void vup::VAO::render(GLenum render_mode) {
    bind();
    glDrawArrays(render_mode, 0, m_count);
}

void vup::VAO::render(GLenum render_mode, int offset, unsigned int count) {
    bind();
    glDrawArrays(render_mode, offset, count);
}
