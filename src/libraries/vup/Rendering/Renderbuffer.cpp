//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Renderbuffer.h"

vup::Renderbuffer::Renderbuffer(vup::FBO_attachment t, int width, int height) {
    glCreateRenderbuffers(1, &m_id);
    m_target = t.target;
    m_format = t.format;
    m_sized_format = t.sized_format;
    m_type = t.type;
    resize(width, height);
}

void vup::Renderbuffer::resize(int width, int height) {
    m_width = width;
    m_height = height;
    glNamedRenderbufferStorage(m_id, gl::to_gl(m_format), m_width, m_height);
}

GLuint vup::Renderbuffer::get_id() {
    return m_id;
}
