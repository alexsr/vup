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
        const std::vector<vup::Mesh>& get_meshes();
        vup::Mesh get_mesh(unsigned long i);
    private:
        std::vector<vup::Mesh> m_meshes;
    };
}

#endif //VUP_MESH_H
