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
#include <iostream>

namespace vup
{
    class Buffer {
    public:
        template <typename T>
        void update_data(const std::vector<T> &data);
        template <typename T>
        void set_data(const std::vector<T> &data);
        template <typename T>
        void set_data(const std::vector<T> &data, GLbitfield flags);
        GLuint get_name() const;
        unsigned int get_buffer_size() const;
        void bind();
        void unbind();
    protected:
        explicit Buffer(GLenum target, GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        template <typename T>
        explicit Buffer(GLenum target, const std::vector<T> &data, GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        GLuint m_name = 0;
        GLenum m_storage_flags;
        GLenum m_target = 0;
        bool m_dynamically_updatable = false;
        unsigned int m_buffer_size = 0;
    };
}

template<typename T>
vup::Buffer::Buffer(GLenum target, const std::vector<T> &data, GLbitfield flags)
        : m_target(target), m_storage_flags(flags) {
    glCreateBuffers(1, &m_name);
    m_buffer_size = sizeof(T) * data.size();
    glNamedBufferStorage(m_name, m_buffer_size, data.data(), m_storage_flags);
    m_dynamically_updatable = static_cast<bool>(flags & GL_DYNAMIC_STORAGE_BIT);
}

template<typename T>
void vup::Buffer::update_data(const std::vector<T>& data) {
    GLvoid* buffer_ptr = glMapNamedBufferRange(m_name, 0, m_buffer_size,
                                               GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    std::memcpy(buffer_ptr, data.data(), static_cast<size_t>(m_buffer_size));
    glUnmapNamedBuffer(m_name);
}

template<typename T>
void vup::Buffer::set_data(const std::vector<T>& data) {
    if (!m_dynamically_updatable) {
        throw std::runtime_error{"Buffer " + std::to_string(m_name) + " is not dynamically updatable."};
    }
    glNamedBufferSubData(m_name, 0, m_buffer_size, data.data());
}

template<typename T>
void vup::Buffer::set_data(const std::vector<T>& data, GLbitfield flags) {
    m_storage_flags = flags;
    set_data(data);
}

#endif //VUP_BUFFER_H
