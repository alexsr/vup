//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_VBO_H
#define VUP_VBO_H

#include "Buffer.h"

namespace vup
{
    class VBO : public vup::Buffer {
    public:
        explicit VBO(GLint vertex_size = 4, GLenum type = GL_FLOAT, GLenum draw_usage = GL_STATIC_DRAW);
        template <typename T>
        explicit VBO(const std::vector<T>& data, GLint vertex_size = 4,
                     GLenum format = GL_FLOAT, GLenum draw_usage = GL_STATIC_DRAW);
        GLint get_vertex_size() const;
        GLenum get_format() const;
        int get_format_size() const;
        int get_stride() const;
    private:
        int determine_format_size();
        GLint m_vertex_size;
        GLenum m_format;
        int m_format_size;
        int m_stride;
    };
}

template<typename T>
vup::VBO::VBO(const std::vector<T>& data, GLint vertex_size, GLenum format, GLenum draw_usage)
        : vup::Buffer(GL_ARRAY_BUFFER, data, draw_usage), m_vertex_size(vertex_size),
          m_format(format) {
    m_format_size = determine_format_size();
    m_stride = m_format_size * m_vertex_size;
}

#endif //VUP_VBO_H