//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Mesh_loader.h"

vup::Mesh_loader::Mesh_loader(const filesystem::path& path) {
    if (!filesystem::exists(path)) {
        throw std::runtime_error{"File at " + path.string() + " not found."};
    }
    if (filesystem::is_empty(path)) {
        throw std::runtime_error{"File at " + path.string() + " is empty."};
    }
    std::cout << "Loading mesh from file: " << path.string() << "\n";
    Assimp::Importer imp;
    auto scene = imp.ReadFile(path.string(), 0);
	if ((scene == nullptr) || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mNumMeshes <= 0) {
		throw std::runtime_error{"Mesh file not working. Path: " + path.string() + "\n" + imp.GetErrorString()};
    }
    for (int i = 0; i < scene->mNumMeshes; i++) {
        add_mesh(scene->mMeshes[i]);

    }
}

const std::vector<vup::Mesh_data>& vup::Mesh_loader::get_data_of_meshes() {
    return m_data_of_meshes;
}

vup::Mesh_data vup::Mesh_loader::get_mesh_data(unsigned long i) {
    return m_data_of_meshes.at(i);
}

void vup::Mesh_loader::add_mesh(const aiMesh * m) {
    Mesh_data mesh{};
    mesh.count = m->mNumVertices;
    mesh.faces_count = m->mNumFaces;
#pragma omp parallel for
    mesh.vertices.resize(mesh.count);
    for(unsigned long i = 0; i < mesh.count; i++) {
        mesh.vertices.at(i) = glm::vec4(m->mVertices[i].x, m->mVertices[i].y, m->mVertices[i].z, 1);
    }
    mesh.normals.resize(mesh.count);
    if (m->HasNormals()) {
#pragma omp parallel for
        for(unsigned long i = 0; i < mesh.count; i++) {
            mesh.normals.at(i) = glm::vec4(m->mNormals[i].x, m->mNormals[i].y, m->mNormals[i].z, 0);
        }
    }
    else {
        std::cout << "Mesh has no normals.\n";
    }
    mesh.uv_coords.resize(mesh.count);
    if (m->HasTextureCoords(0)) {
#pragma omp parallel for
        for(unsigned long i = 0; i < mesh.count; i++) {
            mesh.uv_coords.at(i) = glm::vec2(m->mTextureCoords[0][i].x, m->mTextureCoords[0][i].y);
        }
    }
    else {
        std::cout << "Mesh has no uv coords.\n";
    }

    mesh.indices.resize(mesh.faces_count * 3);
#pragma omp parallel for
    for(unsigned int i = 0; i < mesh.faces_count; i++) {
        aiFace face = m->mFaces[i];
#pragma omp parallel for
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            mesh.indices.at(i * face.mNumIndices + j) = face.mIndices[j];
        }
    }
    m_data_of_meshes.push_back(mesh);
}
