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
        explicit Element_buffer(gl::Storage flags = gl::Storage::dynamic | gl::Storage::write);
        explicit Element_buffer(const std::vector<unsigned int>& indices,
                                gl::Storage flags = gl::Storage::dynamic | gl::Storage::write);
        void set_data(const std::vector<unsigned int>& data);
        unsigned long get_count() const;
    private:
        unsigned long m_count;
    };
}

#endif //VUP_ELEMENT_BUFFER_H

#pragma clang diagnostic pop