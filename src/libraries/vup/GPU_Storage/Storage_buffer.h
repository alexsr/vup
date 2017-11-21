//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_STORAGE_BUFFER_H
#define VUP_STORAGE_BUFFER_H

#include "Buffer.h"

namespace vup
{
    class Storage_buffer : public vup::Buffer {
    protected:
        explicit Storage_buffer(GLenum target, GLuint binding = 0,
                                gl::storage flags = gl::storage::dynamic | gl::storage::write);
        Storage_buffer(GLenum target, GLuint binding, unsigned int size,
                                gl::storage flags = gl::storage::dynamic | gl::storage::write);
        template <typename T>
        explicit Storage_buffer(GLenum target, const T& data, GLuint binding = 0,
                                gl::storage flags = gl::storage::dynamic | gl::storage::write);
        void bind_base(GLuint binding);
        GLuint m_binding;
    };

    class UBO : public Storage_buffer {
    public:
        explicit UBO(GLuint binding = 0, gl::storage flags = gl::storage::dynamic | gl::storage::write);
        UBO(GLuint binding, unsigned int size, gl::storage flags = gl::storage::dynamic | gl::storage::write);
        template <typename T>
        explicit UBO(const T& data, GLuint binding = 0, gl::storage flags = gl::storage::dynamic | gl::storage::write);
    };

    class SSBO : public Storage_buffer {
    public:
        explicit SSBO(GLuint binding = 0, gl::storage flags = gl::storage::dynamic | gl::storage::write);
        SSBO(GLuint binding, unsigned int size, gl::storage flags = gl::storage::dynamic | gl::storage::write);
        template <typename T>
        explicit SSBO(const T& data, GLuint binding = 0, gl::storage flags = gl::storage::dynamic | gl::storage::write);
    };
}

template<typename T>
vup::Storage_buffer::Storage_buffer(GLenum target, const T& data, GLuint binding, gl::storage flags)
        : vup::Buffer(target, data, flags), m_binding(binding) {
    bind_base(binding);
}

template<typename T>
vup::UBO::UBO(const T& data, GLuint binding, gl::storage flags)
        : vup::Storage_buffer(GL_UNIFORM_BUFFER, data, binding, flags) {
}

template<typename T>
vup::SSBO::SSBO(const T& data, GLuint binding, gl::storage flags)
        : vup::Storage_buffer(GL_SHADER_STORAGE_BUFFER, data, binding, flags) {
}

#endif //VUP_STORAGE_BUFFER_H
