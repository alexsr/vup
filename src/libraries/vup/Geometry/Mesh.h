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
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <iostream>
#include <map>
#include <string>

namespace vup
{
    struct Mesh_data {
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec4> normals;
        std::vector<glm::vec2> uv_coords;
        std::vector<unsigned int> indices;
        unsigned int count;
        unsigned int faces_count;
    };

    class Mesh {
    public:
        explicit Mesh(const Mesh_data& m);
        const std::vector<vup::VBO>& get_VBOs() const;
        vup::VBO get_VBO(unsigned int i);
        vup::Element_buffer get_indices();
        unsigned int get_count() const;
    private:
        std::vector<vup::VBO> m_vbos;
        vup::Element_buffer m_index_buffer;
        unsigned int m_count;
        unsigned int m_faces_count;
    };
}

#endif //VUP_MESH_H
