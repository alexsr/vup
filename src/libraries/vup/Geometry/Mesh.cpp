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
    m_vbos.push_back(std::make_shared<VBO>(m.vertices, 4));
    m_vbos.push_back(std::make_shared<VBO>(m.normals, 4));
    m_vbos.push_back(std::make_shared<VBO>(m.uv_coords, 2));
    m_index_buffer = std::make_shared<Element_buffer>(m.indices);
}

std::shared_ptr<vup::VBO> vup::Mesh::get_vbo(const unsigned int i) const {
    return m_vbos.at(i);
}

std::shared_ptr<vup::Element_buffer> vup::Mesh::get_index_buffer() const {
    return m_index_buffer;
}

unsigned int vup::Mesh::get_count() const {
    return m_count;
}

unsigned vup::Mesh::get_vbo_count() const {
    return static_cast<unsigned int>(m_vbos.size());
}
