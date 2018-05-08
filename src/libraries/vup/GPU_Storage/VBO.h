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
    class VBO : public Buffer {
    public:
        explicit VBO(GLint vertex_size = 4, GLenum format = GL_FLOAT,
                     gl::storage flags = gl::storage::dynamic | gl::storage::write);
        template <typename T>
        explicit VBO(const T& data, GLint vertex_size = 4, GLenum format = GL_FLOAT,
                     gl::storage flags = gl::storage::dynamic | gl::storage::write);
        template <typename T>
        explicit VBO(const std::vector<T>& data, GLint vertex_size = 4, GLenum format = GL_FLOAT,
                     gl::storage flags = gl::storage::dynamic | gl::storage::write);
        GLint get_vertex_size() const;
        GLenum get_format() const;
        int get_format_size() const;
        int get_stride() const;
        void bind_base(GLuint binding) const;
    private:
        static constexpr int determine_format_size(GLenum format);
        GLint m_vertex_size;
        GLenum m_format;
        int m_format_size;
        int m_stride;
    };

    template <typename T>
    VBO::VBO(const T& data, const GLint vertex_size, const GLenum format, const gl::storage flags)
        : Buffer(GL_ARRAY_BUFFER, data, flags), m_vertex_size(vertex_size),
          m_format(format) {
        m_format_size = determine_format_size(format);
        m_stride = m_format_size * m_vertex_size;
    }

    template <typename T>
    VBO::VBO(const std::vector<T>& data, const GLint vertex_size, const GLenum format, const gl::storage flags)
        : Buffer(GL_ARRAY_BUFFER, data, flags), m_vertex_size(vertex_size),
          m_format(format) {
        m_format_size = determine_format_size(format);
        m_stride = m_format_size * m_vertex_size;
    }

    class Instanced_VBO : public VBO {
    public:
        explicit Instanced_VBO(GLint vertex_size = 4, GLuint divisor = 1, GLenum type = GL_FLOAT,
                               gl::storage flags = gl::storage::dynamic | gl::storage::write);
        template <typename T>
        explicit Instanced_VBO(const std::vector<T>& data, GLint vertex_size = 4,
                               GLuint divisor = 1, GLenum type = GL_FLOAT,
                               gl::storage flags = gl::storage::dynamic | gl::storage::write);
        GLuint get_divisor() const;
    private:
        GLuint m_divisor;
    };

    template <typename T>
    Instanced_VBO::Instanced_VBO(const std::vector<T>& data, const GLint vertex_size,
                                 const GLuint divisor, const GLenum type, const gl::storage flags)
        : VBO(data, vertex_size, type, flags), m_divisor(divisor) {
    }
}

#endif //VUP_VBO_H
