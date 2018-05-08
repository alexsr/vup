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
#include "VBO.h"
#include <memory>

namespace vup
{
    class Base_VAO {
    public:
        virtual ~Base_VAO();
        void bind() const;
        static void unbind();
        void delete_vao() const;
        std::shared_ptr<VBO> get_vbo(unsigned long i) const;
    protected:
        explicit Base_VAO(const std::initializer_list<std::shared_ptr<VBO>>& vbos = {});
        explicit Base_VAO(const std::shared_ptr<VBO>& main_vbo,
                          const std::initializer_list<std::shared_ptr<VBO>>& vbos = {});
        explicit Base_VAO(const Geometric_primitive& primitive,
                          const std::initializer_list<std::shared_ptr<VBO>>& vbos = {});
        explicit Base_VAO(const Mesh& mesh,
                          const std::initializer_list<std::shared_ptr<VBO>>& vbos = {});
        void set_attrib_buffer(const std::shared_ptr<VBO>& v, unsigned int i);
        void set_attrib_format(unsigned int index, GLint vertex_size, GLenum format) const;
        GLuint m_name = 0;
        unsigned int m_count = 0;
        std::vector<std::shared_ptr<VBO>> m_vbos;
    };

    class VAO : public Base_VAO {
    public:
        explicit VAO(const std::initializer_list<std::shared_ptr<VBO>>& vbos = {});
        explicit VAO(const std::shared_ptr<VBO>& main_vbo,
                     const std::initializer_list<std::shared_ptr<VBO>>& vbos = {});
        explicit VAO(const Geometric_primitive& primitive,
                     const std::initializer_list<std::shared_ptr<VBO>>& vbos = {});
        explicit VAO(const Mesh& mesh,
                     const std::initializer_list<std::shared_ptr<VBO>>& vbos = {});
        virtual void render(GLenum render_mode) const;
        virtual void render(GLenum render_mode, int offset, unsigned int count) const;
    };

    class Instanced_VAO : public Base_VAO {
    public:
        explicit Instanced_VAO(const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {});
        explicit Instanced_VAO(const std::shared_ptr<VBO>& main_vbo,
                               const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {});
        explicit Instanced_VAO(const std::initializer_list<std::shared_ptr<VBO>>& vbos,
                               const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {});
        explicit Instanced_VAO(const std::shared_ptr<VBO>& main_vbo,
                               const std::initializer_list<std::shared_ptr<VBO>>& vbos,
                               const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {});
        explicit Instanced_VAO(const Geometric_primitive& primitive,
                               const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {});
        explicit Instanced_VAO(const Geometric_primitive& primitive,
                               const std::initializer_list<std::shared_ptr<VBO>>& vbos,
                               const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {});
        explicit Instanced_VAO(const Mesh& mesh,
                               const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {});
        virtual void render(GLenum render_mode, unsigned int instances) const;
        virtual void render(GLenum render_mode, int offset, unsigned int count,
                            unsigned int instances) const;
    private:
        void set_divisor_qualifier(const std::shared_ptr<Instanced_VBO>& v, unsigned int index) const;
    };

    class Element_VAO : public VAO {
    public:
        explicit Element_VAO(const std::shared_ptr<Element_buffer>& element_vbo,
                             const std::initializer_list<std::shared_ptr<VBO>>& vbos = {});
        explicit Element_VAO(const std::shared_ptr<VBO>& main_vbo,
                             const std::shared_ptr<Element_buffer>& element_vbo,
                             const std::initializer_list<std::shared_ptr<VBO>>& vbos = {});
        explicit Element_VAO(const Geometric_primitive& primitive,
                             const std::initializer_list<std::shared_ptr<VBO>>& vbos = {});
        explicit Element_VAO(const Mesh& mesh);
        void render(GLenum render_mode) const override;
        void render(GLenum render_mode, int offset, unsigned int count) const override;
    };

    class Instanced_element_VAO : public Instanced_VAO {
    public:

        explicit Instanced_element_VAO(const std::shared_ptr<Element_buffer>& element_vbo,
                                       const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {});
        explicit Instanced_element_VAO(const std::shared_ptr<VBO>& main_vbo,
                                       const std::shared_ptr<Element_buffer>& element_vbo,
                                       const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {
                                       });
        explicit Instanced_element_VAO(const std::shared_ptr<Element_buffer>& element_vbo,
                                       const std::initializer_list<std::shared_ptr<VBO>>& vbos = {},
                                       const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {
                                       });
        explicit Instanced_element_VAO(const std::shared_ptr<VBO>& main_vbo,
                                       const std::shared_ptr<Element_buffer>& element_vbo,
                                       const std::initializer_list<std::shared_ptr<VBO>>& vbos,
                                       const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {
                                       });
        explicit Instanced_element_VAO(const Geometric_primitive& primitive,
                                       const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {
                                       });
        explicit Instanced_element_VAO(const Geometric_primitive& primitive,
                                       const std::initializer_list<std::shared_ptr<VBO>>& vbos,
                                       const std::initializer_list<std::shared_ptr<Instanced_VBO>>& instanced_vbos = {
                                       });
        void render(GLenum render_mode, unsigned int instances) const override;
        void render(GLenum render_mode, int offset, unsigned int count,
                    unsigned int instances) const override;
        const std::shared_ptr<VBO> main_vbo;
    };
}

#endif //VUP_VAO_H
