//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_VAO_H
#define VUP_VAO_H

#include <vup/Geometry/Mesh.h>
#include <vup/Geometry/Geometric_primitives.h>
#include <initializer_list>
#include "Instanced_VBO.h"
#include "Element_buffer.h"

namespace vup
{
    class Base_VAO {
    public:
        virtual ~Base_VAO() = default;
        void bind() const;
        static void unbind();
        void delete_vao() const;
        VBO get_vbo(unsigned long i) const;
    protected:
        explicit Base_VAO(const std::initializer_list<VBO>& vbos = {});
        explicit Base_VAO(const VBO& main_vbo,
                          const std::initializer_list<VBO>& vbos = {});
        explicit Base_VAO(const Geometric_primitive& primitive,
                          const std::initializer_list<VBO>& vbos = {});
        explicit Base_VAO(const Mesh& mesh,
                          const std::initializer_list<VBO>& vbos = {});
        void set_attrib_buffer(const VBO& v, unsigned int i);
        void set_attrib_format(unsigned int index, GLint vertex_size, GLenum format) const;
        GLuint m_name = 0;
        unsigned int m_count = 0;
        std::vector<VBO> m_vbos;
    };

    class VAO : public Base_VAO {
    public:
        explicit VAO(const std::initializer_list<VBO>& vbos = {});
        explicit VAO(const VBO& main_vbo,
                     const std::initializer_list<VBO>& vbos = {});
        explicit VAO(const Geometric_primitive& primitive,
                     const std::initializer_list<VBO>& vbos = {});
        explicit VAO(const Mesh& mesh,
                     const std::initializer_list<VBO>& vbos = {});
        virtual void render(GLenum render_mode) const;
        virtual void render(GLenum render_mode, int offset, unsigned int count) const;
    };

    class Instanced_VAO : public Base_VAO {
    public:
        explicit Instanced_VAO(const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_VAO(const VBO& main_vbo,
                               const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_VAO(const std::initializer_list<VBO>& vbos,
                               const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_VAO(const VBO& main_vbo,
                               const std::initializer_list<VBO>& vbos,
                               const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_VAO(const Geometric_primitive& primitive,
                               const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_VAO(const Geometric_primitive& primitive,
                               const std::initializer_list<VBO>& vbos,
                               const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_VAO(const Mesh& mesh,
                               const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        virtual void render(GLenum render_mode, unsigned int instances) const;
        virtual void render(GLenum render_mode, int offset, unsigned int count,
                            unsigned int instances) const;
    private:
        void set_divisor_qualifier(const Instanced_VBO& v, unsigned int index) const;
    };

    class Element_VAO : public VAO {
    public:
        explicit Element_VAO(const Element_buffer& element_vbo,
                             const std::initializer_list<VBO>& vbos = {});
        explicit Element_VAO(const VBO& main_vbo, const Element_buffer& element_vbo,
                             const std::initializer_list<VBO>& vbos = {});
        explicit Element_VAO(const Geometric_primitive& primitive,
                             const std::initializer_list<VBO>& vbos = {});
        explicit Element_VAO(Mesh mesh);
        void render(GLenum render_mode) const override;
        void render(GLenum render_mode, int offset, unsigned int count) const override;
    };

    class Instanced_element_VAO : public Instanced_VAO {
    public:

        explicit Instanced_element_VAO(const Element_buffer& element_vbo,
                                       const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_element_VAO(const VBO& main_vbo,
                                       const Element_buffer& element_vbo,
                                       const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_element_VAO(const Element_buffer& element_vbo, const std::initializer_list<VBO>& vbos = {},
                                       const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_element_VAO(const VBO& main_vbo,
                                       const Element_buffer& element_vbo,
                                       const std::initializer_list<VBO>& vbos,
                                       const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_element_VAO(const Geometric_primitive& primitive,
                                       const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_element_VAO(const Geometric_primitive& primitive,
                                       const std::initializer_list<VBO>& vbos,
                                       const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        void render(GLenum render_mode, unsigned int instances) const override;
        void render(GLenum render_mode, int offset, unsigned int count,
                    unsigned int instances) const override;
    };
}

#endif //VUP_VAO_H
