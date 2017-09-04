//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Buffer.h"

vup::Buffer::Buffer(GLenum target, GLenum draw_usage) : m_target(target), m_draw_usage(draw_usage) {
    glCreateBuffers(1, &m_name);
}

GLuint vup::Buffer::get_name() const {
    return m_name;
}

unsigned int vup::Buffer::get_buffer_size() const {
    return m_buffer_size;
}

void vup::Buffer::bind() {
    glBindBuffer(m_target, m_name);
}

void vup::Buffer::unbind() {
    glBindBuffer(m_target, 0);
}

