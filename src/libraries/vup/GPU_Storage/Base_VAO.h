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
#include <initializer_list>

namespace vup
{
    class Base_VAO {
    public:
        virtual ~Base_VAO() = default;
        void bind();
        void unbind();
        void delete_vao();
        VBO get_vbo(unsigned long i);
    protected:
        explicit Base_VAO(const VBO& main_vbo,
                          const std::initializer_list<VBO>& vbos = {});
        explicit Base_VAO(const Geometric_primitive& primitive,
                          const std::initializer_list<VBO>& vbos = {});
        explicit Base_VAO(const Mesh& mesh,
                          const std::initializer_list<VBO>& vbos = {});
        void set_attrib_buffer(const VBO& v, unsigned int i);
        void set_attrib_format(unsigned int index, GLint vertex_size, GLenum format);
        GLuint m_name = 0;
        unsigned int m_count = 0;
        std::vector<VBO> m_vbos;
    };
}

#endif //VUP_BASE_VAO_H
