//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_BUFFER_H
#define VUP_BUFFER_H

#include <vup/Core/vup.h>
#include <vup/Core/gl_utils.h>
#include <vector>
#include <cstring>
#include <iostream>
#include <string>

namespace vup
{
    class Buffer {
    public:
        template <typename T>
        void update_data(const T& data);
        template <typename T>
        void update_data(const std::vector<T>& data);
        template <typename T>
        void set_data(const T& data);
        GLuint get_name() const;
        unsigned int get_buffer_size() const;
        void bind();
        void unbind();
    protected:
        explicit Buffer(GLenum target, gl::storage flags = gl::storage::dynamic | gl::storage::write);
        template <typename T>
        explicit Buffer(GLenum target, const T& data, gl::storage flags = gl::storage::dynamic | gl::storage::write);
        template <typename T>
        explicit Buffer(GLenum target, const std::vector<T>& data, gl::storage flags = gl::storage::dynamic | gl::storage::write);
        void initialize_empty_storage(unsigned int size);
        template <typename T>
        void initialize_storage(const T& data);
        template <typename T>
        void initialize_storage(const std::vector<T>& data);
        GLuint m_name = 0;
        gl::storage m_storage_flags;
        GLenum m_target = 0;
        bool m_dynamically_updatable = false;
        bool m_storage_initialized = false;
        unsigned int m_buffer_size = 0;
    };
}

template<typename T>
vup::Buffer::Buffer(GLenum target, const T& data, gl::storage flags)
        : m_target(target), m_storage_flags(flags), m_storage_initialized(true) {
    glCreateBuffers(1, &m_name);
    initialize_storage(data);
    m_dynamically_updatable = m_storage_flags & gl::storage::dynamic;
}

template<typename T>
vup::Buffer::Buffer(GLenum target, const std::vector<T>& data, gl::storage flags)
        : m_target(target), m_storage_flags(flags), m_storage_initialized(true) {
    glCreateBuffers(1, &m_name);
    initialize_storage(data);
    m_dynamically_updatable = m_storage_flags & gl::storage::dynamic;
}

template<typename T>
void vup::Buffer::update_data(const T& data) {
    if (!m_dynamically_updatable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not dynamically updatable."};
    }
    GLvoid* buffer_ptr = glMapNamedBufferRange(m_name, 0, m_buffer_size,
                                               GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    std::memcpy(buffer_ptr, &data, static_cast<size_t>(m_buffer_size));
    glUnmapNamedBuffer(m_name);
}

template<typename T>
void vup::Buffer::update_data(const std::vector<T>& data) {
    if (!m_dynamically_updatable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not dynamically updatable."};
    }
    GLvoid* buffer_ptr = glMapNamedBufferRange(m_name, 0, m_buffer_size,
                                               GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    std::memcpy(buffer_ptr, data.data(), static_cast<size_t>(m_buffer_size));
    glUnmapNamedBuffer(m_name);
}

template<typename T>
void vup::Buffer::set_data(const T& data) {
    if (!m_storage_initialized) {
        initialize_storage(data);
    }
    update_data(data);
}

template <typename T>
void vup::Buffer::initialize_storage(const T& data) {
    m_buffer_size = sizeof(T);
    glNamedBufferStorage(m_name, m_buffer_size, &data, gl::cast_to_bit(m_storage_flags));
    m_storage_initialized = true;
}

template <typename T>
void vup::Buffer::initialize_storage(const std::vector<T>& data) {
    m_buffer_size = sizeof(T) * data.size();
    glNamedBufferStorage(m_name, m_buffer_size, data.data(), gl::cast_to_bit(m_storage_flags));
    m_storage_initialized = true;
}

#endif //VUP_BUFFER_H
