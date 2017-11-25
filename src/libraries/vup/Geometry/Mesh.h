//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_MESH_H
#define VUP_MESH_H

#include <vup/GPU_Storage/VBO.h>
#include <vup/GPU_Storage/Element_buffer.h>
#include <assimp/mesh.h>
#include <iostream>
#include <map>
#include <string>

namespace vup
{
    class Mesh {
    public:
        explicit Mesh(const aiMesh* m);
        std::vector<vup::VBO> get_VBOs();
        vup::VBO get_VBO(unsigned int i);
        vup::Element_buffer get_indices();
        unsigned int get_count();
    private:
        std::vector<vup::VBO> m_vbos;
        vup::Element_buffer m_index_buffer;
        unsigned int m_count;
        void copy_3D_data(const aiVector3D* data,
                                  unsigned int size);
        void copy_2D_data(const aiVector3D* data, unsigned int size);
    };
}

#endif //VUP_MESH_H
