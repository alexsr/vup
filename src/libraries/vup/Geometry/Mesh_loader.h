//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_MESH_LOADER_H
#define VUP_MESH_LOADER_H

#include <string_view>
#include <stdexcept>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "Mesh.h"

namespace vup
{
    class Mesh_loader {
    public:
        Mesh_loader(std::string_view path);
        const std::vector<Mesh>& get_meshes();
        Mesh get_mesh(unsigned long i);
    private:
        std::vector<Mesh> m_meshes;
    };
}

#endif //VUP_MESH_H
