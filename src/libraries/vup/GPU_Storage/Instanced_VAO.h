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
    class Instanced_VAO : public vup::Base_VAO {
    public:
        explicit Instanced_VAO(const vup::VBO& main_vbo,
                               const std::initializer_list<vup::VBO>& vbos,
                               const std::initializer_list<vup::Instanced_VBO>& instanced_vbos);
        explicit Instanced_VAO(const vup::VBO& main_vbo,
                               const std::initializer_list<vup::Instanced_VBO>& instanced_vbos = {});
        virtual void render(GLenum render_mode, unsigned int instances);
        virtual void render(GLenum render_mode, int offset, unsigned int count,
                            unsigned int instances);
    private:
        void set_divisor_qualifier(const vup::Instanced_VBO& v,
                                                  unsigned int index);
    };
}

#endif //VUP_INSTANCED_VAO_H
