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
        void update_data(const T& data, int offset, int size);
        template <typename T>
        void update_data(const std::vector<T>& data, int offset);
        template <typename T>
        void set_data(const T& data);
        GLuint get_name() const;
        unsigned int get_buffer_size() const;
        void bind();
        void unbind();
        void delete_buffer();
    protected:
        explicit Buffer(GLenum target, gl::Storage flags = gl::Storage::dynamic | gl::Storage::write);
        template <typename T>
        explicit Buffer(GLenum target, const T& data, gl::Storage flags = gl::Storage::dynamic | gl::Storage::write);
        template <typename T>
        explicit Buffer(GLenum target, const std::vector<T>& data, gl::Storage flags = gl::Storage::dynamic | gl::Storage::write);
        void initialize_empty_storage(unsigned int size);
        template <typename T>
        void initialize_storage(const T& data);
        template <typename T>
        void initialize_storage(const std::vector<T>& data);
        GLuint m_name = 0;
        gl::Storage m_storage_flags;
        GLenum m_target = 0;
        bool m_dynamically_updatable = false;
        bool m_storage_initialized = false;
        unsigned int m_buffer_size = 0;
    };
}

template<typename T>
vup::Buffer::Buffer(GLenum target, const T& data, gl::Storage flags)
        : m_target(target), m_storage_flags(flags), m_storage_initialized(true) {
    glCreateBuffers(1, &m_name);
    initialize_storage(data);
    m_dynamically_updatable = m_storage_flags & gl::Storage::dynamic;
}

template<typename T>
vup::Buffer::Buffer(GLenum target, const std::vector<T>& data, gl::Storage flags)
        : m_target(target), m_storage_flags(flags), m_storage_initialized(true) {
    glCreateBuffers(1, &m_name);
    initialize_storage(data);
    m_dynamically_updatable = m_storage_flags & gl::Storage::dynamic;
}

template<typename T>
void vup::Buffer::update_data(const T& data) {
    if (!m_dynamically_updatable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not dynamically updatable."};
    }
    GLvoid* buffer_ptr = glMapNamedBufferRange(m_name, 0, m_buffer_size,
                                               gl::cast_to_bit(gl::Access::write | gl::Access::invalidate_buffer));
    std::memcpy(buffer_ptr, &data, static_cast<size_t>(m_buffer_size));
    glUnmapNamedBuffer(m_name);
}

template<typename T>
void vup::Buffer::update_data(const std::vector<T>& data) {
    if (!m_dynamically_updatable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not dynamically updatable."};
    }
    GLvoid* buffer_ptr = glMapNamedBufferRange(m_name, 0, m_buffer_size,
                                               gl::cast_to_bit(gl::Access::write | gl::Access::invalidate_buffer));
    std::memcpy(buffer_ptr, data.data(), static_cast<size_t>(m_buffer_size));
    glUnmapNamedBuffer(m_name);
}

template<typename T>
void vup::Buffer::update_data(const T& data, int offset, int size) {
    if (!m_dynamically_updatable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not dynamically updatable."};
    }
    if (offset < 0 || size < 0 || offset + size > m_buffer_size) {
        throw std::runtime_error{"Offset or size are impractical to update buffer" + std::to_string(m_name) + "."};
    }
    GLvoid* buffer_ptr = glMapNamedBufferRange(m_name, offset, size,
        gl::cast_to_bit(gl::Access::write | gl::Access::invalidate_buffer));
    std::memcpy(buffer_ptr, &data, static_cast<size_t>(size));
    glUnmapNamedBuffer(m_name);
}

template<typename T>
void vup::Buffer::update_data(const std::vector<T>& data, int offset) {
    if (!m_dynamically_updatable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not dynamically updatable."};
    }
    if (offset < 0 || offset + data.size() > m_buffer_size) {
        throw std::runtime_error{"Offset or size are impractical to update buffer" + std::to_string(m_name) + "."};
    }
    GLvoid* buffer_ptr = glMapNamedBufferRange(m_name, offset, data.size(),
        gl::cast_to_bit(gl::Access::write | gl::Access::invalidate_buffer));
    std::memcpy(buffer_ptr, data.data(), static_cast<size_t>(data.size()));
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
