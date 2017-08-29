//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_INSTANCED_VBO_H
#define VUP_INSTANCED_VBO_H

#include "VBO.h"

namespace vup
{
    class Instanced_VBO : public vup::VBO {
    public:
        explicit Instanced_VBO(GLuint divisor = 1, GLint vertex_size = 4, GLenum type = GL_FLOAT, GLenum draw_usage = GL_STATIC_DRAW);
        template <typename T>
        explicit Instanced_VBO(const std::vector<T> &data, GLuint divisor = 1, GLint vertex_size = 4,
                               GLenum type = GL_FLOAT, GLenum draw_usage = GL_STATIC_DRAW);
        GLuint get_divisor() const;
    private:
        GLuint m_divisor;
    };

    template<typename T>
    vup::Instanced_VBO::Instanced_VBO(const std::vector<T>& data, GLuint divisor,
                                      GLint vertex_size, GLenum type, GLenum draw_usage)
            : VBO(data, vertex_size, type, draw_usage), m_divisor(divisor) {

    }
}

#endif //VUP_INSTANCED_VBO_H
