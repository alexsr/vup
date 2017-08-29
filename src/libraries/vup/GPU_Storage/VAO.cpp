//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "VAO.h"

vup::VAO::VAO(const std::initializer_list<vup::VBO>& vbos) {
    glCreateVertexArrays(1, &m_name);
    unsigned int i = 0;
    for (auto&& v : vbos) {
        glEnableVertexArrayAttrib(m_name, i);
        glVertexArrayVertexBuffer(m_name, i, v.get_name(), 0, v.get_stride());
        set_attrib_format(i, v.get_vertex_size(), v.get_format());
        glVertexArrayAttribBinding(m_name, i, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

void vup::VAO::bind() {
    glBindVertexArray(m_name);
}

void vup::VAO::unbind() {
    glBindVertexArray(0);
}

void vup::VAO::set_attrib_format(unsigned int index, GLint vertex_size, GLenum format) {
    if (format == GL_DOUBLE) {
        glVertexArrayAttribLFormat(m_name, index, vertex_size, format, 0);
    }
    else if (format == GL_BYTE || format == GL_SHORT
             || format == GL_INT || format == GL_UNSIGNED_BYTE
             || format == GL_UNSIGNED_SHORT || format == GL_UNSIGNED_INT) {
        glVertexArrayAttribIFormat(m_name, index, vertex_size, format, 0);
    }
    else {
        glVertexArrayAttribFormat(m_name, index, vertex_size, format, GL_FALSE, 0);
    }
}

void vup::VAO::set_divisor_qualifier(const vup::Instanced_VBO& v,
                                     unsigned int index) {
    glVertexArrayBindingDivisor(m_name, index, v.get_divisor());
}

void vup::VAO::set_divisor_qualifier(const vup::VBO& v,
                                     unsigned int index) {

}
