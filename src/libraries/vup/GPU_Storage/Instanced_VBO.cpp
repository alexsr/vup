//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Instanced_VBO.h"

vup::Instanced_VBO::Instanced_VBO(GLint vertex_size, GLuint divisor, GLenum type, GLenum draw_usage)
        : VBO(vertex_size, type, draw_usage), m_divisor(divisor) {

}

GLuint vup::Instanced_VBO::get_divisor() const {
    return m_divisor;
}
