//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Shader.h"

vup::Shader::Shader(std::string path, GLenum type) {
    m_path = std::move(path);
    m_type = type;
    m_shader_id = glCreateShader(type);
    load_shader(m_path);
}

void vup::Shader::reload() {
    load_shader(m_path);
}

void vup::Shader::load_shader(const std::string &path) {
    vup::File_loader f(path);
    const GLchar* source = f.get_source().c_str();
    GLint size = f.get_size();
    glShaderSource(m_shader_id, 1, &source, &size);
    glCompileShader(m_shader_id);
    GLint success = 0;
    glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint log_size = 0;
        glGetShaderiv(m_shader_id, GL_INFO_LOG_LENGTH, &log_size);
        std::vector<GLchar> error_log(log_size);
        glGetShaderInfoLog(m_shader_id, log_size, &log_size, &error_log[0]);
        glDeleteShader(m_shader_id);
        throw std::runtime_error{"Error while compiling "
                                 + vup::shader_type_to_string(m_type) + ".\n"
                                 + "Path: " + m_path + "\n"
                                 + "Error log: \n"
                                 + std::string(error_log.begin(), error_log.end())};
    }
}
