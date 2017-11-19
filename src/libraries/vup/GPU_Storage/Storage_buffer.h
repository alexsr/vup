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
        explicit Storage_buffer(GLenum target, GLuint binding = 0, unsigned int size = 0,
                                GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        template <typename T>
        explicit Storage_buffer(GLenum target, const T& data, GLuint binding = 0,
                                GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        void bind_base(GLuint binding);
        GLuint m_binding;
    };

    class UBO : public Storage_buffer {
    public:
        explicit UBO(GLint binding = 0, unsigned int size = 0,
                     GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        template <typename T>
        explicit UBO(const T& data, GLuint binding = 0, GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    };

    class SSBO : public Storage_buffer {
    public:
        explicit SSBO(GLint binding = 0, unsigned int size = 0,
                      GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        template <typename T>
        explicit SSBO(const T& data, GLuint binding = 0, GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    };
}

template<typename T>
vup::Storage_buffer::Storage_buffer(GLenum target, const T& data, GLuint binding, GLbitfield flags)
        : vup::Buffer(target, data, flags), m_binding(binding) {
    bind_base(binding);
}

template<typename T>
vup::UBO::UBO(const T& data, GLuint binding, GLbitfield flags)
        : vup::Storage_buffer(GL_UNIFORM_BUFFER, data, binding, flags) {
}

template<typename T>
vup::SSBO::SSBO(const T& data, GLuint binding, GLbitfield flags)
        : vup::Storage_buffer(GL_SHADER_STORAGE_BUFFER, data, binding, flags) {
}

#endif //VUP_STORAGE_BUFFER_H
