//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_BUFFER_H
#define VUP_BUFFER_H

#include <vup/Core/vup.h>
#include <vector>
#include <cstring>
#include <string>
#include "storage_utils.h"

namespace vup
{
    class Buffer {
    public:
        template <typename T>
        void update_data(const T& data);
        template <typename T>
        void update_data(const std::vector<T>& data);
        template <typename T>
        void update_data(const T& data, int offset, int size);
        template <typename T>
        void update_data(const std::vector<T>& data, int offset);
        template <typename T>
        void set_data(const T& data);
        template <typename T>
        void get_data(T& data);
        template <typename T>
        void get_data(std::vector<T>& data);
        template <typename T>
        void get_data(T& data, int offset);
        template <typename T>
        void get_data(std::vector<T>& data, int offset);
        GLuint get_name() const;
        unsigned int get_buffer_size() const;
        void bind();
        void unbind();
        void delete_buffer();
        bool is_initialized() const;
    protected:
        explicit Buffer(GLenum target, gl::storage flags = gl::storage::dynamic | gl::storage::write);
        template <typename T>
        explicit Buffer(GLenum target, const T& data, gl::storage flags = gl::storage::dynamic | gl::storage::write);
        template <typename T>
        explicit Buffer(GLenum target, const std::vector<T>& data,
                        gl::storage flags = gl::storage::dynamic | gl::storage::write);
        void initialize_empty_storage(unsigned int size);
        template <typename T>
        void initialize_storage(const T& data);
        template <typename T>
        void initialize_storage(const std::vector<T>& data);
        GLuint m_name = 0;
        GLenum m_target = 0;
        gl::storage m_storage_flags;
        bool m_dynamically_updatable = false;
        bool m_readable = false;
        bool m_storage_initialized = false;
        unsigned int m_buffer_size = 0;
    };
}

template <typename T>
vup::Buffer::Buffer(GLenum target, const T& data, gl::storage flags)
    : m_target(target), m_storage_flags(flags), m_storage_initialized(true) {
    glCreateBuffers(1, &m_name);
    initialize_storage(data);
    m_dynamically_updatable = m_storage_flags & gl::storage::dynamic && m_storage_flags & gl::storage::write;
    m_readable = m_storage_flags & gl::storage::read;
}

template <typename T>
vup::Buffer::Buffer(GLenum target, const std::vector<T>& data, gl::storage flags)
    : m_target(target), m_storage_flags(flags), m_storage_initialized(true) {
    glCreateBuffers(1, &m_name);
    initialize_storage(data);
    m_dynamically_updatable = m_storage_flags & gl::storage::dynamic && m_storage_flags & gl::storage::write;
    m_readable = m_storage_flags & gl::storage::read;
}

template <typename T>
void vup::Buffer::update_data(const T& data) {
    if (!m_dynamically_updatable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not dynamically updatable."};
    }
    const auto buffer_ptr = glMapNamedBufferRange(m_name, 0, m_buffer_size,
                                                  to_gl(gl::map_access::write | gl::map_access::invalidate_buffer));
    std::memcpy(buffer_ptr, &data, static_cast<size_t>(m_buffer_size));
    glUnmapNamedBuffer(m_name);
}

template <typename T>
void vup::Buffer::update_data(const std::vector<T>& data) {
    if (!m_dynamically_updatable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not dynamically updatable."};
    }
    size_t size = data.size() * sizeof(T);
    if (size > m_buffer_size) {
        size = m_buffer_size;
    }
    auto buffer_ptr = glMapNamedBufferRange(m_name, 0, size,
                                            to_gl(gl::map_access::write | gl::map_access::invalidate_buffer));
    std::memcpy(buffer_ptr, data.data(), size);
    glUnmapNamedBuffer(m_name);
}

template <typename T>
void vup::Buffer::update_data(const T& data, int offset, int size) {
    if (!m_dynamically_updatable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not dynamically updatable."};
    }
    if (offset < 0 || size < 0 || offset + size > m_buffer_size) {
        throw std::runtime_error{"Offset or size are impractical to update buffer" + std::to_string(m_name) + "."};
    }
    const auto buffer_ptr = glMapNamedBufferRange(m_name, offset, size,
                                                  to_gl(gl::map_access::write | gl::map_access::invalidate_buffer));
    std::memcpy(buffer_ptr, &data, static_cast<size_t>(size));
    glUnmapNamedBuffer(m_name);
}

template <typename T>
void vup::Buffer::update_data(const std::vector<T>& data, int offset) {
    if (!m_dynamically_updatable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not dynamically updatable."};
    }
    size_t size = data.size() * sizeof(T);
    if (offset < 0) {
        throw std::runtime_error{"Offset is impractical to update buffer" + std::to_string(m_name) + "."};
    }
    if (offset + size > m_buffer_size) {
        size = m_buffer_size - offset;
    }
    auto buffer_ptr = glMapNamedBufferRange(m_name, offset, size,
                                            to_gl(gl::map_access::write | gl::map_access::invalidate_buffer));
    std::memcpy(buffer_ptr, data.data(), size);
    glUnmapNamedBuffer(m_name);
}

template <typename T>
void vup::Buffer::set_data(const T& data) {
    if (!m_storage_initialized) {
        initialize_storage(data);
    }
    else {
        update_data(data);
    }
}

template <typename T>
void vup::Buffer::get_data(T& data) {
    if (!m_readable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not readable."};
    }
    auto size = sizeof(data);
    if (size > m_buffer_size) {
        size = m_buffer_size;
    }
    auto buffer_ptr = glMapNamedBufferRange(m_name, 0, size,
                                            to_gl(gl::map_access::read));
    std::memcpy(data, buffer_ptr, size);
    glUnmapNamedBuffer(m_name);
}

template <typename T>
void vup::Buffer::get_data(std::vector<T>& data) {
    if (!m_readable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not readable."};
    }
    size_t size = data.size() * sizeof(T);
    if (size > m_buffer_size) {
        size = m_buffer_size;
    }
    auto buffer_ptr = glMapNamedBufferRange(m_name, 0, size,
                                            to_gl(gl::map_access::read));
    std::memcpy(data.data(), buffer_ptr, size);
    glUnmapNamedBuffer(m_name);
}

template <typename T>
void vup::Buffer::get_data(T& data, int offset) {

}

template <typename T>
void vup::Buffer::get_data(std::vector<T>& data, int offset) {

}

template <typename T>
void vup::Buffer::initialize_storage(const T& data) {
    m_buffer_size = sizeof(T);
    glNamedBufferStorage(m_name, m_buffer_size, &data, to_gl(m_storage_flags));
    m_storage_initialized = true;
}

template <typename T>
void vup::Buffer::initialize_storage(const std::vector<T>& data) {
    m_buffer_size = sizeof(T) * data.size();
    glNamedBufferStorage(m_name, m_buffer_size, data.data(), to_gl(m_storage_flags));
    m_storage_initialized = true;
}

#endif //VUP_BUFFER_H
