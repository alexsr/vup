//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "VBO.h"

vup::VBO::VBO(GLint vertex_size, GLenum format, GLbitfield flags)
        : Buffer(GL_ARRAY_BUFFER, flags), m_vertex_size(vertex_size),
          m_format(format) {
    m_format_size = determine_format_size();
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

int vup::VBO::determine_format_size() {
    switch (m_format) {
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
