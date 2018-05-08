//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <vup/Core/gl_utils.h>
#include "Buffer.h"

vup::Buffer::Buffer(const GLenum target, const gl::storage flags) : m_target(target), m_storage_flags(flags) {
    glCreateBuffers(1, &m_name);
    m_dynamically_updatable = m_storage_flags & gl::storage::dynamic && m_storage_flags & gl::storage::write;
    m_readable = m_storage_flags & gl::storage::read;
    m_storage_initialized = false;
}

vup::Buffer::~Buffer() {
    delete_buffer();
}

GLuint vup::Buffer::get_name() const {
    return m_name;
}

unsigned int vup::Buffer::get_buffer_size() const {
    return m_buffer_size;
}

void vup::Buffer::bind() const {
    glBindBuffer(m_target, m_name);
}

void vup::Buffer::unbind() const {
    glBindBuffer(m_target, 0);
}

void vup::Buffer::delete_buffer() const {
    unbind();
    glDeleteBuffers(1, &m_name);
}

bool vup::Buffer::is_initialized() const {
    return m_storage_initialized;
}

void vup::Buffer::initialize_empty_storage(const unsigned int size) {
    m_buffer_size = size;
    glNamedBufferStorage(m_name, m_buffer_size, nullptr, to_gl(m_storage_flags));
    m_storage_initialized = true;
}

vup::Element_buffer::Element_buffer(const gl::storage flags)
    : Buffer(GL_ELEMENT_ARRAY_BUFFER, flags), m_count(0) {
}

vup::Element_buffer::Element_buffer(const std::vector<unsigned int>& indices,
    const gl::storage flags)
    : Buffer(GL_ELEMENT_ARRAY_BUFFER, indices, flags) {
    m_count = static_cast<unsigned int>(indices.size());
}

void vup::Element_buffer::set_data(const std::vector<unsigned int>& data) {
    Buffer::set_data(data);
    m_count = static_cast<int>(data.size());
}

unsigned long vup::Element_buffer::get_count() const {
    return m_count;
}
