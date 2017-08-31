//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Element_buffer.h"

vup::Element_buffer::Element_buffer(GLenum draw_usage)
        : vup::Buffer(GL_ELEMENT_ARRAY_BUFFER, draw_usage) {
}

vup::Element_buffer::Element_buffer(const std::vector<unsigned int>& indices,
                               GLenum draw_usage)
        : vup::Buffer(GL_ELEMENT_ARRAY_BUFFER, indices, draw_usage),
          m_count(static_cast<int>(indices.size())){
}

void vup::Element_buffer::set_data(const std::vector<unsigned int>& data) {
    Buffer::set_data(data);
    m_count = static_cast<int>(data.size());
}

void vup::Element_buffer::set_data(const std::vector<unsigned int>& data,
                                   GLenum draw_usage) {
    Buffer::set_data(data, draw_usage);
    m_count = static_cast<int>(data.size());
}

unsigned int vup::Element_buffer::get_count() const {
    return m_count;
}
