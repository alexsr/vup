//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Storage_buffer.h"

vup::Storage_buffer::Storage_buffer(GLenum target, GLuint base, GLbitfield flags) : vup::Buffer(target, flags) {
    bind_base(base);
}

void vup::Storage_buffer::bind_base(GLuint base) {
    glBindBufferBase(m_target, base, m_name);
}

vup::UBO::UBO(GLuint base, GLbitfield flags)
        : vup::Storage_buffer(GL_UNIFORM_BUFFER, base, flags) {
}

vup::SSBO::SSBO(GLuint base, GLbitfield flags)
        : vup::Storage_buffer(GL_UNIFORM_BUFFER, base, flags) {
}