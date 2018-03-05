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

namespace vup
{
    // Mesh_data is used to store data which is commonly part of a mesh definition.
    struct Mesh_data {
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec4> normals;
        std::vector<glm::vec2> uv_coords;
        std::vector<unsigned int> indices;
        unsigned int count;
        unsigned int faces_count;
    };

    // Mesh creates and stores the VBOs and index buffer for given Mesh_data.
    class Mesh {
    public:
        explicit Mesh(const Mesh_data& m);
        const std::vector<VBO>& get_vbos() const;
        VBO get_vbo(unsigned int i);
        Element_buffer get_index_buffer();
        unsigned int get_count() const;
    private:
        std::vector<VBO> m_vbos;
        Element_buffer m_index_buffer;
        unsigned int m_count;
        unsigned int m_faces_count;
    };
}

#endif //VUP_MESH_H
