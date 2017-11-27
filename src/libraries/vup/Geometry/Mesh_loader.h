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
        const std::vector<vup::Mesh_data>& get_data_of_meshes();
        vup::Mesh_data get_mesh_data(unsigned long i);
    private:
        void add_mesh(const aiMesh* m);
        std::vector<vup::Mesh_data> m_data_of_meshes;
    };
}

#endif //VUP_MESH_H
