//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Instanced_VBO.h"

vup::Instanced_VBO::Instanced_VBO(const GLint vertex_size, const GLuint divisor, const GLenum type, const gl::storage flags)
    : VBO(vertex_size, type, flags), m_divisor(divisor) {

}

GLuint vup::Instanced_VBO::get_divisor() const {
    return m_divisor;
}
