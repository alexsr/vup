//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "VBO.h"

vup::VBO::VBO(const GLint vertex_size, const GLenum format, const gl::storage flags)
    : Buffer(GL_ARRAY_BUFFER, flags), m_vertex_size(vertex_size),
      m_format(format) {
    m_format_size = determine_format_size(format);
    m_stride = m_format_size * m_vertex_size;
}

GLint vup::VBO::get_vertex_size() const {
    return m_vertex_size;
}

GLenum vup::VBO::get_format() const {
    return m_format;
}

int vup::VBO::get_format_size() const {
    return m_format_size;
}

int vup::VBO::get_stride() const {
    return m_stride;
}

void vup::VBO::bind_base(const GLuint binding) const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_name);
}

constexpr int vup::VBO::determine_format_size(const GLenum format) {
    switch (format) {
        case GL_DOUBLE:
            return sizeof(double);
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
            return sizeof(char);
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
            return sizeof(short);
        default:
            return sizeof(float);
    }
}

vup::Instanced_VBO::Instanced_VBO(const GLint vertex_size, const GLuint divisor, const GLenum type, const gl::storage flags)
    : VBO(vertex_size, type, flags), m_divisor(divisor) {

}

GLuint vup::Instanced_VBO::get_divisor() const {
    return m_divisor;
}
