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
#pragma omp parallel for
    for (int i = 0; i < scene->mNumMeshes; i++) {
        m_meshes.at(i) = scene->mMeshes[i];
    }
}

const std::vector<aiMesh*>& vup::Mesh_loader::get_meshes() {
    return m_meshes;
}

aiMesh* vup::Mesh_loader::get_mesh(unsigned long i) {
    return m_meshes.at(i);
}
