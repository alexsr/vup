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
        explicit Instanced_VBO(GLint vertex_size = 4, GLuint divisor = 1, GLenum type = GL_FLOAT,
                               GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        template <typename T>
        explicit Instanced_VBO(const std::vector<T> &data, GLint vertex_size = 4,
                               GLuint divisor = 1, GLenum type = GL_FLOAT,
                               GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        GLuint get_divisor() const;
    private:
        GLuint m_divisor;
    };

    template<typename T>
    vup::Instanced_VBO::Instanced_VBO(const std::vector<T>& data, GLint vertex_size,
                                      GLuint divisor, GLenum type, GLbitfield flags)
            : VBO(data, vertex_size, type, flags), m_divisor(divisor) {

    }
}

#endif //VUP_INSTANCED_VBO_H
