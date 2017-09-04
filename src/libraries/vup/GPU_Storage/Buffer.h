//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_BUFFER_H
#define VUP_BUFFER_H

#include "vup/Core/gl_utils.h"
#include <vector>
#include <cstring>

namespace vup
{
    class Buffer {
    public:
        template <typename T>
        void update_data(const std::vector<T> &data);
        template <typename T>
        void set_data(const std::vector<T> &data);
        template <typename T>
        void set_data(const std::vector<T> &data, GLenum draw_usage);
        GLuint get_name() const;
        unsigned int get_buffer_size() const;
        void bind();
        void unbind();
    protected:
        explicit Buffer(GLenum target, GLenum draw_usage = GL_STATIC_DRAW);
        template <typename T>
        explicit Buffer(GLenum target, const std::vector<T> &data, GLenum draw_usage = GL_STATIC_DRAW);
        GLuint m_name = 0;
        GLenum m_draw_usage = GL_STATIC_DRAW;
        GLenum m_target = 0;
        unsigned int m_buffer_size = 0;
    };
}

template<typename T>
vup::Buffer::Buffer(GLenum target, const std::vector<T> &data, GLenum draw_usage)
        : m_target(target), m_draw_usage(draw_usage) {
    glCreateBuffers(1, &m_name);
    set_data(data);
}

template<typename T>
void vup::Buffer::update_data(const std::vector<T>& data) {
    GLvoid* buffer_ptr = glMapNamedBufferRange(m_name, 0, m_buffer_size,
                                               GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    memcpy(buffer_ptr, data.data(), static_cast<size_t>(m_buffer_size));
    glUnmapNamedBuffer(m_name);
}

template<typename T>
void vup::Buffer::set_data(const std::vector<T>& data) {
    m_buffer_size = sizeof(T) * data.size();
    glNamedBufferData(m_name, m_buffer_size, data.data(), m_draw_usage);
}

template<typename T>
void vup::Buffer::set_data(const std::vector<T>& data, GLenum draw_usage) {
    m_draw_usage = draw_usage;
    set_data(data);
}

#endif //VUP_BUFFER_H
