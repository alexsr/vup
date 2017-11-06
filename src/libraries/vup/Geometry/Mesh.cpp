//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Mesh.h"

vup::Mesh::Mesh(const aiMesh* m) {
    m_count = m->mNumVertices;
    copy_3D_data(m_count, m->mVertices);
    if (m->HasNormals()) {
        copy_3D_data(m_count, m->mNormals);
    }
    else {
        std::cout << "Mesh has no normals.\n";
        m_vbos.emplace_back(std::vector<float>(m->mNumVertices), 3);
    }
    if (m->HasTextureCoords(0)) {
        copy_3D_data(m_count, m->mTextureCoords[0]);
    }
    else {
        m_vbos.emplace_back(std::vector<float>(m_count/3*2), 2);
    }

    std::vector<unsigned int> indices;
    for(unsigned int i = 0; i < m->mNumFaces; i++) {
        aiFace face = m->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    m_index_buffer.set_data(indices);
}

std::vector<vup::VBO> vup::Mesh::get_VBOs() {
    return m_vbos;
}

vup::Element_buffer vup::Mesh::get_indices() {
    return m_index_buffer;
}

unsigned int vup::Mesh::get_count() {
    return m_count;
}

void vup::Mesh::copy_3D_data(unsigned int size, const aiVector3D* data) {
    std::vector<glm::vec3> v(size);
    std::memcpy(v.data(), data, size*sizeof(float)*3);
    m_vbos.emplace_back(v, 3);
}

void vup::Mesh::copy_2D_data(unsigned int size, const aiVector2D* data) {
    std::vector<glm::vec2> v(size);
    auto it = v.begin();
    for(unsigned int i = 0; i < size; i++) {
        it = v.emplace(it, data[i].x, data[i].y);
    }
    m_vbos.emplace_back(v, 2);
}
