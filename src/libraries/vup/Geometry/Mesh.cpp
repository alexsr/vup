//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Mesh.h"

vup::Mesh::Mesh(const Mesh_data& m) {
    m_count = m.count;
    m_faces_count = m.faces_count;
    m_vbos.emplace_back(m.vertices, 4);
    m_vbos.emplace_back(m.normals, 4);
    m_vbos.emplace_back(m.uv_coords, 2);
    m_index_buffer.set_data(m.indices);
}

const std::vector<vup::VBO>& vup::Mesh::get_vbos() const {
    return m_vbos;
}

vup::VBO vup::Mesh::get_vbo(unsigned int i) {
    return m_vbos.at(i);
}

vup::Element_buffer vup::Mesh::get_index_buffer() {
    return m_index_buffer;
}

unsigned int vup::Mesh::get_count() const {
    return m_count;
}
