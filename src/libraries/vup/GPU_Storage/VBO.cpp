//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "VBO.h"

vup::VBO::VBO(GLint vertex_size, GLenum format, GLenum draw_usage)
        : Buffer(GL_ARRAY_BUFFER, draw_usage), m_vertex_size(vertex_size),
          m_format(format), m_stride(vertex_size) {
    calculate_stride();
}

GLint vup::VBO::get_vertex_size() const {
    return m_vertex_size;
}

GLenum vup::VBO::get_format() const {
    return m_format;
}

int vup::VBO::get_stride() const {
    return m_stride;
}

void vup::VBO::calculate_stride() {
    switch (m_format) {
        case GL_DOUBLE:
            m_stride *= sizeof(double);
            break;
        case GL_BYTE:
        case GL_UNSIGNED_BYTE:
            m_stride *= sizeof(char);
            break;
        case GL_SHORT:
        case GL_UNSIGNED_SHORT:
            m_stride *= sizeof(short);
            break;
        default:
            m_stride *= sizeof(float);
    }
}
