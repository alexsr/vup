//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/gl_utils.h>
#include "Buffer.h"

vup::Buffer::Buffer(GLenum target, vup::gl::storage flags) : m_target(target), m_storage_flags(flags) {
    glCreateBuffers(1, &m_name);
    m_dynamically_updatable = m_storage_flags & gl::storage::dynamic;
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

void vup::Buffer::initialize_empty_storage(unsigned int size) {
    m_buffer_size = size;
    glNamedBufferStorage(m_name, m_buffer_size, nullptr, gl::cast_to_bit(m_storage_flags));
    m_storage_initialized = true;
}
