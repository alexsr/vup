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
        explicit Storage_buffer(GLenum target, GLuint base = 0, GLenum draw_usage = GL_STATIC_DRAW);
        template <typename T>
        explicit Storage_buffer(GLenum target, const std::vector<T> &data, GLuint base = 0, GLenum draw_usage = GL_STATIC_DRAW);
        void bind_base(GLuint base);
    };

    class UBO : public Storage_buffer {
    public:
        explicit UBO(GLuint binding, GLenum draw_usage = GL_STATIC_DRAW);
        template <typename T>
        explicit UBO(const std::vector<T> &data, GLuint base = 0, GLenum draw_usage = GL_STATIC_DRAW);
    };

    class SSBO : public Storage_buffer {
    public:
        explicit SSBO(GLuint binding, GLenum draw_usage = GL_STATIC_DRAW);
        template <typename T>
        explicit SSBO(const std::vector<T> &data, GLuint base = 0, GLenum draw_usage = GL_STATIC_DRAW);
    };
}

template<typename T>
vup::Storage_buffer::Storage_buffer(GLenum target, const std::vector<T>& data, GLuint base, GLenum draw_usage)
        :Buffer(target, data, draw_usage) {
    bind_base(base);
}

template<typename T>
vup::UBO::UBO(const std::vector<T>& data, GLuint base, GLenum draw_usage)
        : Storage_buffer(GL_UNIFORM_BUFFER, data, base, draw_usage) {
}

template<typename T>
vup::SSBO::SSBO(const std::vector<T>& data, GLuint base, GLenum draw_usage)
        : Storage_buffer(GL_SHADER_STORAGE_BUFFER, data, base, draw_usage) {
}

#endif //VUP_STORAGE_BUFFER_H
