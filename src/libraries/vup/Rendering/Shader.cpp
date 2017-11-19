//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Shader.h"

vup::Shader::Shader(const filesystem::path& path, GLenum type)
        : m_path(path), m_type(type) {
    m_shader_id = glCreateShader(m_type);
    load_shader(m_path);
}

vup::Shader::~Shader() {
    glDeleteShader(m_shader_id);
}

const GLuint vup::Shader::get_id() const {
    return m_shader_id;
}

const std::string vup::Shader::get_path() const {
    return m_path.string();
}

void vup::Shader::reload() const {
    load_shader(m_path);
}

void vup::Shader::load_shader(const filesystem::path& path) const {
    std::cout << "Loading shader from file: " << path.string() << "\n";
    vup::File_loader f(path);
    const GLchar* source = f.get_source_view().data();
    auto size = static_cast<GLint>(f.get_size());
    glShaderSource(m_shader_id, 1, &source, &size);
    glCompileShader(m_shader_id);
    GLint compile_status = 0;
    glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
        GLint log_size = 0;
        glGetShaderiv(m_shader_id, GL_INFO_LOG_LENGTH, &log_size);
        std::string error_log;
        error_log.resize(static_cast<unsigned long>(log_size));
        glGetShaderInfoLog(m_shader_id, log_size, &log_size, error_log.data());
        glDeleteShader(m_shader_id);
        throw std::runtime_error{"Error while compiling "
                                 + vup::shader_type_to_string(m_type) + ".\n"
                                 + "Path: " + m_path.string() + "\n"
                                 + "Error log: \n"
                                 + error_log};
    }
}

vup::Vertex_shader::Vertex_shader(const filesystem::path& path)
        : vup::Shader(path, GL_VERTEX_SHADER) {
}

vup::Control_shader::Control_shader(const filesystem::path& path)
        : vup::Shader(path, GL_TESS_CONTROL_SHADER) {
}

vup::Evaluation_shader::Evaluation_shader(const filesystem::path& path)
        : vup::Shader(path, GL_TESS_EVALUATION_SHADER) {
}

vup::Geometry_shader::Geometry_shader(const filesystem::path& path)
        : vup::Shader(path, GL_GEOMETRY_SHADER) {
}

vup::Fragment_shader::Fragment_shader(const filesystem::path& path)
        : vup::Shader(path, GL_FRAGMENT_SHADER) {
}
