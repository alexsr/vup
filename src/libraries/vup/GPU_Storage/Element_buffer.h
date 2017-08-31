//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
#ifndef VUP_ELEMENT_BUFFER_H
#define VUP_ELEMENT_BUFFER_H

#include "Buffer.h"

namespace vup
{
    class Element_buffer : public vup::Buffer {
    public:
        explicit Element_buffer(GLenum draw_usage = GL_STATIC_DRAW);
        explicit Element_buffer(const std::vector<unsigned int>& indices,
                                GLenum draw_usage = GL_STATIC_DRAW);
        void set_data(const std::vector<unsigned int> &data);
        void set_data(const std::vector<unsigned int> &data, GLenum draw_usage);
        unsigned int get_count() const;
    private:
        unsigned int m_count;
    };
}

#endif //VUP_ELEMENT_BUFFER_H

#pragma clang diagnostic pop