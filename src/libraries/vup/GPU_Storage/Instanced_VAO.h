//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_INSTANCED_VAO_H
#define VUP_INSTANCED_VAO_H

#include "Base_VAO.h"
#include "Instanced_VBO.h"

namespace vup
{
    class Instanced_VAO : public Base_VAO {
    public:
        explicit Instanced_VAO(const VBO& main_vbo,
                               const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_VAO(const VBO& main_vbo,
                               const std::initializer_list<VBO>& vbos,
                               const std::initializer_list<Instanced_VBO>& instanced_vbos);
        explicit Instanced_VAO(const Geometric_primitive& primitive,
                               const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        explicit Instanced_VAO(const Geometric_primitive& primitive,
                               const std::initializer_list<VBO>& vbos,
                               const std::initializer_list<Instanced_VBO>& instanced_vbos);
        explicit Instanced_VAO(const Mesh& mesh,
                               const std::initializer_list<Instanced_VBO>& instanced_vbos = {});
        virtual void render(GLenum render_mode, unsigned int instances) const;
        virtual void render(GLenum render_mode, int offset, unsigned int count,
                            unsigned int instances) const;
    private:
        void set_divisor_qualifier(const Instanced_VBO& v, unsigned int index) const;
    };
}

#endif //VUP_INSTANCED_VAO_H
