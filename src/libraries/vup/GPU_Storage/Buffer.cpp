//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Buffer.h"

vup::Buffer::Buffer(GLenum target, GLbitfield flags) : m_target(target), m_storage_flags(flags) {
    glCreateBuffers(1, &m_name);
    m_dynamically_updatable = static_cast<bool>(flags & GL_DYNAMIC_STORAGE_BIT);
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
