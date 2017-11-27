//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_BASE_VAO_H
#define VUP_BASE_VAO_H

#include <vup/Geometry/Mesh.h>
#include <vup/Geometry/Geometric_primitives.h>
#include <memory>
#include <initializer_list>

namespace vup
{
    class Base_VAO {
    public:
        void bind();
        void unbind();
        void delete_vao();
    protected:
        explicit Base_VAO(const vup::VBO& main_vbo,
                          const std::initializer_list<vup::VBO>& vbos = {});
        explicit Base_VAO(const vup::Geometric_primitive& primitive,
                          const std::initializer_list<vup::VBO>& vbos = {});
        explicit Base_VAO(const vup::Mesh& mesh);
        void set_attrib_buffer(const vup::VBO& v, unsigned int i);
        void set_attrib_format(unsigned int index, GLint vertex_size, GLenum format);
        GLuint m_name = 0;
        unsigned int m_count = 0;
    };
}

#endif //VUP_BASE_VAO_H
