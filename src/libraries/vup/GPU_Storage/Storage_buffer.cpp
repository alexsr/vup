//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Storage_buffer.h"

vup::Storage_buffer::Storage_buffer(GLenum target, GLuint binding, gl::storage flags)
    : Buffer(target, flags), m_binding(binding) {
    bind_base(binding);
}

vup::Storage_buffer::Storage_buffer(GLenum target, GLuint binding, unsigned int size, gl::storage flags)
    : Buffer(target, flags), m_binding(binding) {
    bind_base(binding);
    initialize_empty_storage(size);
}

void vup::Storage_buffer::bind_base(GLuint binding) {
    glBindBufferBase(m_target, binding, m_name);
}

vup::UBO::UBO(GLuint binding, gl::storage flags)
    : Storage_buffer(GL_UNIFORM_BUFFER, binding, flags) {
}

vup::UBO::UBO(GLuint binding, unsigned int size, gl::storage flags)
    : Storage_buffer(GL_UNIFORM_BUFFER, binding, size, flags) {
}

vup::SSBO::SSBO(GLuint binding, gl::storage flags)
    : Storage_buffer(GL_UNIFORM_BUFFER, binding, flags) {
}

vup::SSBO::SSBO(GLuint binding, unsigned int size, gl::storage flags)
    : Storage_buffer(GL_UNIFORM_BUFFER, binding, size, flags) {
}
