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
        explicit Storage_buffer(GLenum target, GLuint base = 0,
                                GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        template <typename T>
        explicit Storage_buffer(GLenum target, const std::vector<T> &data, GLuint base = 0,
                                GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        void bind_base(GLuint base);
    };

    class UBO : public Storage_buffer {
    public:
        explicit UBO(GLuint binding, GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        template <typename T>
        explicit UBO(const std::vector<T> &data, GLuint base = 0,
                     GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    };

    class SSBO : public Storage_buffer {
    public:
        explicit SSBO(GLuint binding, GLbitfield flags = GL_MAP_WRITE_BIT);
        template <typename T>
        explicit SSBO(const std::vector<T> &data, GLuint base = 0,
                      GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    };
}

template<typename T>
vup::Storage_buffer::Storage_buffer(GLenum target, const std::vector<T>& data, GLuint base, GLbitfield flags)
        :Buffer(target, data, flags) {
    bind_base(base);
}

template<typename T>
vup::UBO::UBO(const std::vector<T>& data, GLuint base, GLbitfield flags)
        : Storage_buffer(GL_UNIFORM_BUFFER, data, base, flags) {
}

template<typename T>
vup::SSBO::SSBO(const std::vector<T>& data, GLuint base, GLbitfield flags)
        : Storage_buffer(GL_SHADER_STORAGE_BUFFER, data, base, flags) {
}

#endif //VUP_STORAGE_BUFFER_H
