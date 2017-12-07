//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Base_VAO.h"

vup::Base_VAO::Base_VAO(const vup::VBO& main_vbo,
                        const std::initializer_list<vup::VBO>& vbos) {
    glCreateVertexArrays(1, &m_name);
    set_attrib_buffer(main_vbo, 0);
    m_count = main_vbo.get_buffer_size() / main_vbo.get_stride();
    unsigned int i = 1;
    for (auto&& v : vbos) {
        set_attrib_buffer(v, i);
        i++;
    }
}

vup::Base_VAO::Base_VAO(const vup::Geometric_primitive& primitive,
                        const std::initializer_list<vup::VBO>& vbos) {
    glCreateVertexArrays(1, &m_name);
    set_attrib_buffer(vup::VBO(primitive.vertices), 0);
    m_count = static_cast<unsigned int>(primitive.vertices.size());
    set_attrib_buffer(vup::VBO(primitive.normals, 3), 1);
    set_attrib_buffer(vup::VBO(primitive.uv_coords, 2), 2);
    unsigned int i = 3;
    for (auto&& v : vbos) {
        set_attrib_buffer(v, i);
        i++;
    }
}


vup::Base_VAO::Base_VAO(const vup::Mesh& mesh,
                        const std::initializer_list<vup::VBO>& vbos) {
    auto mesh_vbos = mesh.get_VBOs();
    glCreateVertexArrays(1, &m_name);
    m_count = mesh.get_count();
    for (unsigned int i = 0; i < mesh_vbos.size(); i++) {
        set_attrib_buffer(mesh_vbos.at(i), i);
    }
    unsigned int i = m_count;
    for (auto&& v : vbos) {
        set_attrib_buffer(v, i);
        i++;
    }
}

void vup::Base_VAO::bind() {
    glBindVertexArray(m_name);
}

void vup::Base_VAO::unbind() {
    glBindVertexArray(0);
}

void vup::Base_VAO::delete_vao() {
    unbind();
    glDeleteVertexArrays(1, &m_name);
}

void vup::Base_VAO::set_attrib_buffer(const vup::VBO& v, unsigned int i) {
    glEnableVertexArrayAttrib(m_name, i);
    glVertexArrayVertexBuffer(m_name, i, v.get_name(), 0, v.get_stride());
    set_attrib_format(i, v.get_vertex_size(), v.get_format());
    glVertexArrayAttribBinding(m_name, i, i);
}

void vup::Base_VAO::set_attrib_format(unsigned int index, GLint vertex_size, GLenum format) {
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
