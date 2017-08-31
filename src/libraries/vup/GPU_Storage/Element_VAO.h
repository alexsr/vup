//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_ELEMENT_VAO_H
#define VUP_ELEMENT_VAO_H

#include "VAO.h"
#include "Element_buffer.h"

namespace vup
{
    class Element_VAO : public vup::VAO {
    public:
        explicit Element_VAO(const vup::VBO& main_vbo, const vup::Element_buffer& element_vbo,
                     const std::initializer_list<vup::VBO>& vbos = {});
        void render(GLenum render_mode) override;
        void render(GLenum render_mode, int offset, unsigned int count) override;
    };
}

#endif //VUP_ELEMENT_VAO_H
