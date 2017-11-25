//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Mesh.h"

vup::Mesh::Mesh(const aiMesh* m) {
    m_count = m->mNumVertices;
    copy_3D_data(m->mVertices, m_count);
    if (m->HasNormals()) {
        copy_3D_data(m->mNormals, m_count);
    }
    else {
        std::cout << "Mesh has no normals.\n";
        m_vbos.emplace_back(std::vector<glm::vec4>(m_count), 4);
    }
    if (m->HasTextureCoords(0)) {
        copy_2D_data(m->mTextureCoords[0], m_count);
    }
    else {
        m_vbos.emplace_back(std::vector<glm::vec2>(m_count), 2);
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

vup::VBO vup::Mesh::get_VBO(unsigned int i) {
    return m_vbos.at(i);
}

vup::Element_buffer vup::Mesh::get_indices() {
    return m_index_buffer;
}

unsigned int vup::Mesh::get_count() {
    return m_count;
}

void vup::Mesh::copy_3D_data(const aiVector3D* data,
                             unsigned int size) {
    std::vector<glm::vec4> v(size);
    #pragma omp parallel for
    for(int i = 0; i < size; i++) {
        v.at(static_cast<unsigned long>(i)) = glm::vec4(data[i].x, data[i].y, data[i].z, 0);
    }
    m_vbos.emplace_back(v, 4);
}

void vup::Mesh::copy_2D_data(const aiVector3D* data, unsigned int size) {
    std::vector<glm::vec2> v(size);
    #pragma omp parallel for
    for(int i = 0; i < size; i++) {
        v.at(static_cast<unsigned long>(i)) = glm::vec2(data[i].x, data[i].y);
    }
    m_vbos.emplace_back(v, 2);
}
