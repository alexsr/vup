//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_MESH_LOADER_H
#define VUP_MESH_LOADER_H

#include "Mesh.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <stdexcept>
#include <experimental/filesystem>

namespace filesystem = std::experimental::filesystem;

namespace vup
{
    class Mesh_loader {
    public:
        explicit Mesh_loader(const filesystem::path& path);
        const std::vector<aiMesh*>& get_meshes();
        aiMesh* get_mesh(unsigned long i);
    private:
        std::vector<aiMesh*> m_meshes;
    };
}

#endif //VUP_MESH_H
