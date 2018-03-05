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
    // The Mesh_loader class uses assimp to load arbitrary meshes.
    // These meshes are stored as Mesh_data structs.
    class Mesh_loader {
    public:
        explicit Mesh_loader(const filesystem::path& path);
        const std::vector<Mesh_data>& get_data_of_meshes();
        Mesh_data get_mesh_data(unsigned long i);
    private:
        // Creates Mesh_data from an aiMesh, while possibly omitting undefined data.
        // So for example if there are no normals defined in aiMesh, Mesh_data will just
        // have (0, 0, 0) as normals.
        void add_mesh(const aiMesh* m);
        std::vector<Mesh_data> m_data_of_meshes;
    };
}

#endif //VUP_MESH_H
