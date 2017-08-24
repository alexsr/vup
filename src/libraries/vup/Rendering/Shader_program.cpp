//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Shader_program.h"

vup::Shader_program::Shader_program() {
    m_program_id = glCreateProgram();
}

vup::Shader_program::~Shader_program() {
    glDeleteProgram(m_program_id);
}

void vup::Shader_program::use() {
    glUseProgram(m_program_id);
}

void vup::Shader_program::link_program() {
    GLint link_status = 0;
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {
        GLint length = 0;
        glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &length);
        std::vector<GLchar> error_log(static_cast<unsigned long>(length));
        glGetProgramInfoLog(m_program_id, length, &length, &error_log[0]);
        glDeleteProgram(m_program_id);
        throw std::runtime_error{"Error while linking shader program "
                                 + vup::shader_type_to_string(m_program_id) +
                                 ".\n"
                                 + "Error log: \n"
                                 + std::string(begin(error_log), end(error_log))};
    }
}
