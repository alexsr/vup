//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_VAO_H
#define VUP_VAO_H

#include <initializer_list>
#include <memory>
#include "VBO.h"
#include "Instanced_VBO.h"

namespace vup
{
    class VAO {
    public:
        explicit VAO(const std::initializer_list<vup::VBO>& vbos);
        void bind();
        void unbind();
    private:
        void set_attrib_format(unsigned int index, GLint vertex_size, GLenum format);
        void set_divisor_qualifier(const vup::Instanced_VBO& v,
                                   unsigned int index);
        void set_divisor_qualifier(const vup::VBO& v,
                                   unsigned int index);
        GLuint m_name;
        std::vector<std::shared_ptr<vup::VBO>> m_vbos;
    };
}

#endif //VUP_VAO_H
