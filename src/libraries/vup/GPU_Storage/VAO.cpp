//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "VAO.h"

vup::Base_VAO::Base_VAO(const std::initializer_list<std::shared_ptr<VBO>>& vbos) {
    glCreateVertexArrays(1, &m_name);
    unsigned int i = 0;
    for (auto&& v : vbos) {
        set_attrib_buffer(v, i);
        if (i == 0) {
            m_count = v->get_buffer_size() / v->get_stride();
        }
        i++;
    }
}

vup::Base_VAO::Base_VAO(const std::shared_ptr<VBO>& main_vbo,
                        const std::initializer_list<std::shared_ptr<VBO>>& vbos) {
    glCreateVertexArrays(1, &m_name);
    set_attrib_buffer(main_vbo, 0);
    m_count = main_vbo->get_buffer_size() / main_vbo->get_stride();
    unsigned int i = 1;
    for (auto&& v : vbos) {
        set_attrib_buffer(v, i);
        i++;
    }
}

vup::Base_VAO::Base_VAO(const Geometric_primitive& primitive,
                        const std::initializer_list<std::shared_ptr<VBO>>& vbos) {
    glCreateVertexArrays(1, &m_name);
    set_attrib_buffer(std::make_shared<VBO>(primitive.vertices), 0);
    m_count = static_cast<unsigned int>(primitive.vertices.size());
    set_attrib_buffer(std::make_shared<VBO>(primitive.normals, 3), 1);
    set_attrib_buffer(std::make_shared<VBO>(primitive.uv_coords, 2), 2);
    unsigned int i = 3;
    for (auto&& v : vbos) {
        set_attrib_buffer(v, i);
        i++;
    }
}

vup::Base_VAO::Base_VAO(const Mesh& mesh,
                        const std::initializer_list<std::shared_ptr<VBO>>& vbos) {
    glCreateVertexArrays(1, &m_name);
    m_count = mesh.get_count();
    for (unsigned int i = 0; i < mesh.get_vbo_count(); i++) {
        set_attrib_buffer(mesh.get_vbo(i), i);
    }
    auto i = m_count;
    for (auto&& v : vbos) {
        set_attrib_buffer(v, i);
        i++;
    }
}

vup::Base_VAO::~Base_VAO() {
    delete_vao();
}

void vup::Base_VAO::bind() const {
    glBindVertexArray(m_name);
}

void vup::Base_VAO::unbind() {
    glBindVertexArray(0);
}

void vup::Base_VAO::delete_vao() const {
    unbind();
    glDeleteVertexArrays(1, &m_name);
}

void vup::Base_VAO::set_attrib_buffer(const std::shared_ptr<VBO>& v, const unsigned int i) {
    glEnableVertexArrayAttrib(m_name, i);
    glVertexArrayVertexBuffer(m_name, i, v->get_name(), 0, v->get_stride());
    set_attrib_format(i, v->get_vertex_size(), v->get_format());
    glVertexArrayAttribBinding(m_name, i, i);
    m_vbos.push_back(v);

}

void vup::Base_VAO::set_attrib_format(const unsigned int index, const GLint vertex_size, const GLenum format) const {
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

std::shared_ptr<vup::VBO> vup::Base_VAO::get_vbo(const unsigned long i) const {
    return m_vbos.at(i);
}

vup::VAO::VAO(const std::initializer_list<std::shared_ptr<VBO>>& vbos) : Base_VAO(vbos) {
}

vup::VAO::VAO(const std::shared_ptr<VBO>& main_vbo, const std::initializer_list<std::shared_ptr<VBO>>& vbos)
    : Base_VAO(main_vbo, vbos) {
}

vup::VAO::VAO(const Geometric_primitive& primitive, const std::initializer_list<std::shared_ptr<VBO>>& vbos)
    : Base_VAO(primitive, vbos) {
}

vup::VAO::VAO(const Mesh& mesh, const std::initializer_list<std::shared_ptr<VBO>>& vbos) : Base_VAO(mesh, vbos) {
}

void vup::VAO::render(const GLenum render_mode) const {
    bind();
    glDrawArrays(render_mode, 0, m_count);
}

void vup::VAO::render(const GLenum render_mode, const int offset, const unsigned int count) const {
    bind();
    glDrawArrays(render_mode, offset, count);
}

vup::Instanced_VAO::Instanced_VAO(const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos)
    : Base_VAO({}) {
    unsigned int i = 0;
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const std::shared_ptr<VBO>& main_vbo,
                                  const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos)
    : Base_VAO(main_vbo, {}) {
    unsigned int i = 1;
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const std::initializer_list<std::shared_ptr<VBO>>& vbos,
                                  const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos)
    : Base_VAO(vbos) {
    auto i = static_cast<unsigned int>(vbos.size());
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const std::shared_ptr<VBO>& main_vbo,
                                  const std::initializer_list<std::shared_ptr<VBO>>& vbos,
                                  const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos)
    : Base_VAO(main_vbo, vbos) {
    auto i = static_cast<unsigned int>(vbos.size() + 1);
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const Geometric_primitive& primitive,
                                  const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos)
    : Base_VAO(primitive) {
    unsigned int i = 3;
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const Geometric_primitive& primitive,
                                  const std::initializer_list<std::shared_ptr<VBO>>& vbos,
                                  const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos)
    : Base_VAO(primitive, vbos) {
    auto i = static_cast<unsigned int>(vbos.size() + 3);
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

vup::Instanced_VAO::Instanced_VAO(const Mesh& mesh,
                                  const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos)
    : Base_VAO(mesh) {
    auto i = static_cast<unsigned int>(mesh.get_vbo_count());
    for (auto&& v : instanced_vbos) {
        set_attrib_buffer(v, i);
        set_divisor_qualifier(v, i);
        i++;
    }
}

void vup::Instanced_VAO::set_divisor_qualifier(const std::shared_ptr<Instanced_VBO>& v,
                                               const unsigned int index) const {
    glVertexArrayBindingDivisor(m_name, index, v->get_divisor());
}

void vup::Instanced_VAO::render(const GLenum render_mode, const unsigned int instances) const {
    bind();
    glDrawArraysInstanced(render_mode, 0, m_count, instances);
}

void vup::Instanced_VAO::render(const GLenum render_mode, const int offset, const unsigned int count,
                                const unsigned int instances) const {
    bind();
    glDrawArraysInstanced(render_mode, offset, count, instances);
}

vup::Element_VAO::Element_VAO(const std::shared_ptr<Element_buffer>& element_vbo,
                              const std::initializer_list<std::shared_ptr<VBO>>& vbos)
    : VAO(vbos) {
    glVertexArrayElementBuffer(m_name, element_vbo->get_name());
    m_count = element_vbo->get_count();
}

vup::Element_VAO::Element_VAO(const std::shared_ptr<VBO>& main_vbo,
                              const std::shared_ptr<Element_buffer>& element_vbo,
                              const std::initializer_list<std::shared_ptr<VBO>>& vbos)
    : VAO(main_vbo, vbos) {
    glVertexArrayElementBuffer(m_name, element_vbo->get_name());
    m_count = element_vbo->get_count();
}

vup::Element_VAO::Element_VAO(const Geometric_primitive& primitive,
                              const std::initializer_list<std::shared_ptr<VBO>>& vbos)
    : VAO(primitive, vbos) {
    Element_buffer element_vbo(primitive.indices);
    glVertexArrayElementBuffer(m_name, element_vbo.get_name());
    m_count = element_vbo.get_count();
}

vup::Element_VAO::Element_VAO(const Mesh& mesh) : VAO(mesh) {
    glVertexArrayElementBuffer(m_name, mesh.get_index_buffer()->get_name());
    m_count = mesh.get_index_buffer()->get_count();
}

void vup::Element_VAO::render(const GLenum render_mode) const {
    bind();
    glDrawElements(render_mode, m_count, GL_UNSIGNED_INT, nullptr);
}

void vup::Element_VAO::render(const GLenum render_mode, const int offset, const unsigned int count) const {
    bind();
    glDrawElements(render_mode, count, GL_UNSIGNED_INT, &offset);
}

vup::Instanced_element_VAO::Instanced_element_VAO(const std::shared_ptr<Element_buffer>& element_vbo,
                                                  const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos) :
    Instanced_VAO(instanced_vbos) {
    glVertexArrayElementBuffer(m_name, element_vbo->get_name());
    m_count = element_vbo->get_count();
}

vup::Instanced_element_VAO::Instanced_element_VAO(const std::shared_ptr<VBO>& main_vbo,
                                                  const std::shared_ptr<Element_buffer>& element_vbo,
                                                  const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos)
    : Instanced_VAO(main_vbo, instanced_vbos), main_vbo(std::move(main_vbo)) {
    glVertexArrayElementBuffer(m_name, element_vbo->get_name());
    m_count = element_vbo->get_count();
}

vup::Instanced_element_VAO::Instanced_element_VAO(const std::shared_ptr<Element_buffer>& element_vbo,
                                                  const std::initializer_list<std::shared_ptr<VBO>>& vbos,
                                                  const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos)
    : Instanced_VAO(vbos, instanced_vbos) {
    glVertexArrayElementBuffer(m_name, element_vbo->get_name());
    m_count = element_vbo->get_count();
}

vup::Instanced_element_VAO::Instanced_element_VAO(const std::shared_ptr<VBO>& main_vbo,
                                                  const std::shared_ptr<Element_buffer>& element_vbo,
                                                  const std::initializer_list<std::shared_ptr<VBO>>& vbos,
                                                  const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos)
    : Instanced_VAO(main_vbo, vbos, instanced_vbos), main_vbo(std::move(main_vbo)) {
    glVertexArrayElementBuffer(m_name, element_vbo->get_name());
    m_count = element_vbo->get_count();
}

vup::Instanced_element_VAO::Instanced_element_VAO(const Geometric_primitive& primitive,
                                                  const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos)
    : Instanced_VAO(primitive, instanced_vbos) {
    Element_buffer element_vbo(primitive.indices);
    glVertexArrayElementBuffer(m_name, element_vbo.get_name());
    m_count = element_vbo.get_count();
}

vup::Instanced_element_VAO::Instanced_element_VAO(const Geometric_primitive& primitive,
                                                  const std::initializer_list<std::shared_ptr<VBO>>& vbos,
                                                  const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos)
    : Instanced_VAO(primitive, vbos, instanced_vbos) {
    Element_buffer element_vbo(primitive.indices);
    glVertexArrayElementBuffer(m_name, element_vbo.get_name());
    m_count = element_vbo.get_count();
}

void vup::Instanced_element_VAO::render(const GLenum render_mode, const unsigned int instances) const {
    bind();
    glDrawElementsInstanced(render_mode, m_count, GL_UNSIGNED_INT, nullptr, instances);
}

void
vup::Instanced_element_VAO::render(const GLenum render_mode, const int offset, const unsigned int count,
                                   const unsigned int instances) const {
    bind();
    glDrawElementsInstanced(render_mode, count, GL_UNSIGNED_INT, &offset, instances);
}
