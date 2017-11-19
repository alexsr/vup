//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Storage_buffer.h"

vup::Storage_buffer::Storage_buffer(GLenum target, GLuint binding, unsigned int size, GLbitfield flags)
        : vup::Buffer(target, flags), m_binding(binding) {
    bind_base(binding);
    initialize_storage(size, flags);
}

void vup::Storage_buffer::bind_base(GLuint binding) {
    glBindBufferBase(m_target, binding, m_name);
}

vup::UBO::UBO(GLint binding, unsigned int size, GLbitfield flags)
        : vup::Storage_buffer(GL_UNIFORM_BUFFER, binding, size, flags) {
}

vup::SSBO::SSBO(GLint binding, unsigned int size, GLbitfield flags)
        : vup::Storage_buffer(GL_UNIFORM_BUFFER, binding, size, flags) {
}