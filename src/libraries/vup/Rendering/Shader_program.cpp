//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include <iostream>
#include "Shader_program.h"

vup::Shader_program::Shader_program() {
    m_program_id = glCreateProgram();
}

vup::Shader_program::~Shader_program() {
    glDeleteProgram(m_program_id);
}

void vup::Shader_program::use() const {
    glUseProgram(m_program_id);
}

void vup::Shader_program::link_program() const {
    glLinkProgram(m_program_id);
    GLint link_status = 0;
    glGetProgramiv(m_program_id, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {
        GLint length = 0;
        glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &length);
        std::string error_log;
        error_log.resize(static_cast<unsigned long>(length));
        glGetProgramInfoLog(m_program_id, length, &length, &error_log[0]);
        glDeleteProgram(m_program_id);
        throw std::runtime_error{"Error while linking shader program "
                                 + vup::shader_type_to_string(m_program_id) +
                                 ".\n"
                                 + "Error log: \n"
                                 + error_log};
    }
}

void vup::Shader_program::update_uniform(const GLchar* name, bool b) {
    glProgramUniform1i(m_program_id, find_uniform_location(name), b);
}

void vup::Shader_program::update_uniform(const GLchar* name, int i) {
    glProgramUniform1i(m_program_id, find_uniform_location(name), i);
}

void vup::Shader_program::update_uniform(const GLchar* name, float f) {
    glProgramUniform1f(m_program_id, find_uniform_location(name), f);
}

void vup::Shader_program::update_uniform(const GLchar* name, double d) {
    glProgramUniform1d(m_program_id, find_uniform_location(name), d);
}

void vup::Shader_program::update_uniform(const GLchar* name, glm::vec2 v) {
    glProgramUniform2fv(m_program_id, find_uniform_location(name), 1, glm::value_ptr(v));
}

void vup::Shader_program::update_uniform(const GLchar* name, glm::vec3 v) {
    glProgramUniform3fv(m_program_id, find_uniform_location(name), 1, glm::value_ptr(v));
}

void vup::Shader_program::update_uniform(const GLchar* name, glm::vec4 v) {
    glProgramUniform4fv(m_program_id, find_uniform_location(name), 1, glm::value_ptr(v));
}

void vup::Shader_program::update_uniform(const GLchar* name, glm::ivec2 v) {
    glProgramUniform2iv(m_program_id, find_uniform_location(name), 1, glm::value_ptr(v));
}

void vup::Shader_program::update_uniform(const GLchar* name, glm::ivec3 v) {
    glProgramUniform3iv(m_program_id, find_uniform_location(name), 1, glm::value_ptr(v));
}

void vup::Shader_program::update_uniform(const GLchar* name, glm::ivec4 v) {
    glProgramUniform4iv(m_program_id, find_uniform_location(name), 1, glm::value_ptr(v));
}

void vup::Shader_program::update_uniform(const GLchar* name,
                                         std::vector<glm::vec2> v) {
    glProgramUniform2fv(m_program_id, find_uniform_location(name),
                        static_cast<GLsizei>(v.size()), glm::value_ptr(v.data()[0]));
}

void vup::Shader_program::update_uniform(const GLchar* name,
                                         std::vector<glm::vec3> v) {
    glProgramUniform3fv(m_program_id, find_uniform_location(name),
                        static_cast<GLsizei>(v.size()), glm::value_ptr(v.data()[0]));
}

void vup::Shader_program::update_uniform(const GLchar* name,
                                         std::vector<glm::vec4> v) {
    glProgramUniform4fv(m_program_id, find_uniform_location(name),
                        static_cast<GLsizei>(v.size()), glm::value_ptr(v.data()[0]));
}

void vup::Shader_program::update_uniform(const GLchar* name, glm::mat2 m) {
    glProgramUniformMatrix2fv(m_program_id, find_uniform_location(name), 1,
                              GL_FALSE, glm::value_ptr(m));
}

void vup::Shader_program::update_uniform(const GLchar* name, glm::mat3 m) {
    glProgramUniformMatrix3fv(m_program_id, find_uniform_location(name), 1,
                              GL_FALSE, glm::value_ptr(m));
}

void vup::Shader_program::update_uniform(const GLchar* name, glm::mat4 m) {
    glProgramUniformMatrix4fv(m_program_id, find_uniform_location(name), 1,
                              GL_FALSE, glm::value_ptr(m));
}

GLint vup::Shader_program::find_uniform_location(const GLchar* name) {
    GLint loc = glGetUniformLocation(m_program_id, name);
    if (loc == -1) {
        std::cout << "Uniform not found.\n";
    }
    return loc;
}
