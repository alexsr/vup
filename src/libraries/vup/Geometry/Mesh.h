//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_MESH_H
#define VUP_MESH_H

#include <iostream>
#include <assimp/mesh.h>
#include <vup/GPU_Storage/VBO.h>
#include <vup/GPU_Storage/Element_buffer.h>

namespace vup
{
    class Mesh {
    public:
        explicit Mesh(const aiMesh* m);
        std::vector<VBO> get_VBOs();
        vup::Element_buffer get_indices();
        unsigned int get_count();
    private:
        std::vector<VBO> m_vbos;
        vup::Element_buffer m_index_buffer;
        unsigned int m_count;
        void copy_3D_data(unsigned int size, const aiVector3D* data);
        void copy_2D_data(unsigned int size, const aiVector2D* data);
    };
}

#endif //VUP_MESH_H
