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
        explicit VAO(const vup::VBO& main_vbo, const std::initializer_list<vup::VBO>& vbos = {});
        void bind();
        void unbind();
        void render(GLenum render_mode);
    private:
        void set_attrib_buffer(const vup::VBO& v, unsigned int i);
        void set_attrib_format(unsigned int index, GLint vertex_size, GLenum format);
        void set_divisor_qualifier(const vup::Instanced_VBO& v,
                                   unsigned int index);
        void set_divisor_qualifier(const vup::VBO& v,
                                   unsigned int index);
        GLuint m_name = 0;
        GLsizei m_count = 0;
        std::vector<std::shared_ptr<vup::VBO>> m_vbos;
    };
}

#endif //VUP_VAO_H
