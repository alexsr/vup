//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_MESH_H
#define VUP_MESH_H

#include <vup/GPU_Storage/VBO.h>
#include <vup/GPU_Storage/Buffer.h>
#include <memory>

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
        std::shared_ptr<VBO> get_vbo(unsigned int i) const;
        std::shared_ptr<Element_buffer> get_index_buffer() const;
        unsigned int get_count() const;
        unsigned int get_vbo_count() const;
    private:
        std::vector<std::shared_ptr<VBO>> m_vbos;
        std::shared_ptr<Element_buffer> m_index_buffer;
        unsigned int m_count;
        unsigned int m_faces_count;
    };
}

#endif //VUP_MESH_H
