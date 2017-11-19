//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Mesh_loader.h"

vup::Mesh_loader::Mesh_loader(const std::string& path) {
    std::cout << "Loading mesh from file: " << path.data() << "\n";
    Assimp::Importer imp;
    auto scene = imp.ReadFile(path, 0);
	if ((scene == nullptr) || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mNumMeshes <= 0) {
		throw std::runtime_error{"Mesh file not found. Path: " + path + "\n" + imp.GetErrorString()};
    }
    for (int i = 0; i < scene->mNumMeshes; i++) {
        m_meshes.emplace_back(scene->mMeshes[i]);
    }
}

const std::vector<vup::Mesh>& vup::Mesh_loader::get_meshes() {
    return m_meshes;
}

vup::Mesh vup::Mesh_loader::get_mesh(unsigned long i) {
    return m_meshes.at(i);
}
