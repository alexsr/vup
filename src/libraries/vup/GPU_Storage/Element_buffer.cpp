//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Element_buffer.h"

vup::Element_buffer::Element_buffer(gl::storage flags)
    : Buffer(GL_ELEMENT_ARRAY_BUFFER, flags), m_count(0) {
}

vup::Element_buffer::Element_buffer(const std::vector<unsigned int>& indices,
                                    gl::storage flags)
    : Buffer(GL_ELEMENT_ARRAY_BUFFER, indices, flags) {
    m_count = indices.size();
}

void vup::Element_buffer::set_data(const std::vector<unsigned int>& data) {
    Buffer::set_data(data);
    m_count = static_cast<int>(data.size());
}

unsigned long vup::Element_buffer::get_count() const {
    return m_count;
}
