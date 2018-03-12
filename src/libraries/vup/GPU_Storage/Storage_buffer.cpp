//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Storage_buffer.h"

vup::Storage_buffer::Storage_buffer(const GLenum target, const GLuint binding, const gl::storage flags)
    : Buffer(target, flags), m_binding(binding) {
    bind_base(binding);
}

void vup::Storage_buffer::bind_base(const GLuint binding) const {
    glBindBufferBase(m_target, binding, m_name);
}

vup::Empty_storage_buffer::Empty_storage_buffer(const unsigned int size, const GLenum target, const GLuint binding,
                                                const gl::storage flags)
    : Storage_buffer(target, binding, flags) {
    initialize_empty_storage(size);
}

vup::UBO::UBO(const GLuint binding, const gl::storage flags)
    : Storage_buffer(GL_UNIFORM_BUFFER, binding, flags) {
}

vup::Empty_UBO::Empty_UBO(const unsigned int size, const GLuint binding, const gl::storage flags)
    : Empty_storage_buffer(size, GL_UNIFORM_BUFFER, binding, flags) {
}

vup::SSBO::SSBO(const GLuint binding, const gl::storage flags)
    : Storage_buffer(GL_SHADER_STORAGE_BUFFER, binding, flags) {
}

vup::Empty_SSBO::Empty_SSBO(const unsigned int size, const GLuint binding, const gl::storage flags)
    : Empty_storage_buffer(size, GL_SHADER_STORAGE_BUFFER, binding, flags) {
}
