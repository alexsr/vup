//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Shader.h"

vup::Shader::Shader(std::vector<Shader_define> defines,
                    const gl::introspection introspection_flag)
    : m_introspection_flag(introspection_flag), m_defines(std::move(defines)) {
    m_program_id = glCreateProgram();
}

GLuint vup::Shader::load_shader(const filesystem::path& path, const GLenum type) {
    std::cout << "Loading shader from file: " << path.string() << "\n";
    auto f_source = load_file_str(path);
    const auto version_start = f_source.find("#version", 0);
    auto start = f_source.find('\n', version_start) + 1;
    std::string define_string;
    for (auto& d : m_defines) {
        define_string += "#define " + d.name + " " + d.value + "\n";
    }
    f_source.insert(start, define_string);
    const auto main_start = f_source.find("void main");
    process_includes(f_source, path, start, main_start);
    const GLchar* source = f_source.data();
    auto size = static_cast<GLint>(f_source.size());
    const auto shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &source, &size);
    glCompileShader(shader_id);
    GLint compile_status = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE) {
        GLint log_size = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_size);
        std::string error_log;
        error_log.resize(static_cast<unsigned long>(log_size));
        glGetShaderInfoLog(shader_id, log_size, &log_size, error_log.data());
        glDeleteShader(shader_id);
        throw std::runtime_error{
            "Error while compiling "
            + gl::shader_type_to_string(type) + ".\n"
            + "Path: " + path.string() + "\n"
            + "Error log: \n"
            + error_log
        };
    }
    return shader_id;
}

void vup::Shader::process_includes(std::string& file, const filesystem::path& directory,
                                   unsigned long long start, unsigned long long end_search) {
    while ((start = file.find("#include", start)) <= end_search) {
        const auto path_start = file.find('\"', start);
        const auto path_end = file.find('\"', path_start + 1);
        auto path_inc(directory.parent_path() / file.substr(path_start + 1, path_end - path_start - 1));
        std::cout << "Including " << path_inc.string() << "\n";
        const auto eol = file.find('\n', start);
        auto inc = load_file_str(path_inc);
        path_inc = filesystem::canonical(path_inc);
        process_includes(inc, path_inc, 0, inc.size());
        file.replace(start, eol - start, inc.data());
        end_search += inc.size();
    }
}

void vup::Shader::use() const {
    glUseProgram(m_program_id);
}

void vup::Shader::link_program() const {
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
        throw std::runtime_error{
            "Error while linking shader program "
            + gl::shader_type_to_string(m_program_id) +
            ".\n"
            + "Error log: \n"
            + error_log
        };
    }
}

void vup::Shader::init_shader_program(const std::vector<GLuint>& shader_ids) {
    attach_shaders(shader_ids);
    link_program();
    detach_shaders(shader_ids);
    if (m_introspection_flag != gl::introspection::none) {
        inspect_uniforms();
    }
    delete_shaders(shader_ids);
}

void vup::Shader::attach_shaders(const std::vector<GLuint>& shader_ids) const {
    for (auto id : shader_ids) {
        glAttachShader(m_program_id, id);
    }
}

void vup::Shader::detach_shaders(const std::vector<GLuint>& shader_ids) const {
    for (auto id : shader_ids) {
        glDetachShader(m_program_id, id);
    };
}

void vup::Shader::delete_shaders(const std::vector<GLuint>& shader_ids) {
    for (auto id : shader_ids) {
        glDeleteShader(id);
    };
}

void vup::Shader::inspect_uniforms() {
    GLint uniform_count = 0;
    glGetProgramInterfaceiv(m_program_id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniform_count);
    std::array<GLenum, 4> properties{GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION};
    for (int i = 0; i < uniform_count; i++) {
        std::array<GLint, 4> info{};
        glGetProgramResourceiv(m_program_id, GL_UNIFORM, i, 4, properties.data(), 4, nullptr, info.data());
        if (info.at(0) != -1) {
            continue; // skip block uniforms
        }
        std::vector<char> name_data(static_cast<unsigned long>(info.at(2)));
        glGetProgramResourceName(m_program_id, GL_UNIFORM, i, static_cast<GLsizei>(name_data.size()),
                                 nullptr, name_data.data());
        const std::string name(name_data.begin(), name_data.end() - 1);
        add_uniform(name, info.at(1), info.at(3));
    }
}

void vup::Shader::add_uniform(const std::string& name, const GLint type,
                              const GLint location) {
    switch (type) {
        case GL_BOOL:
        case GL_INT:
        case GL_SAMPLER_1D:
        case GL_SAMPLER_2D:
        case GL_SAMPLER_3D:
        case GL_SAMPLER_CUBE:
            m_int_uniforms.emplace(name, location);
            break;
        case GL_INT_VEC2:
            m_ivec2_uniforms.emplace(name, location);
            break;
        case GL_INT_VEC3:
            m_ivec3_uniforms.emplace(name, location);
            break;
        case GL_INT_VEC4:
            m_ivec4_uniforms.emplace(name, location);
            break;
        case GL_FLOAT:
            m_float_uniforms.emplace(name, location);
            break;
        case GL_FLOAT_VEC2:
            m_vec2_uniforms.emplace(name, location);
            break;
        case GL_FLOAT_VEC3:
            m_vec3_uniforms.emplace(name, location);
            break;
        case GL_FLOAT_VEC4:
            m_vec4_uniforms.emplace(name, location);
            break;
        case GL_FLOAT_MAT2:
            m_mat2_uniforms.emplace(name, location);
            break;
        case GL_FLOAT_MAT3:
            m_mat3_uniforms.emplace(name, location);
            break;
        case GL_FLOAT_MAT4:
            m_mat4_uniforms.emplace(name, location);
            break;
        default:
            std::cout << "Uniform type unknown: " << type << std::endl;
            break;
    }
}

void vup::Shader::clear_maps() {
    m_int_uniforms.clear();
    m_float_uniforms.clear();
    m_ivec2_uniforms.clear();
    m_ivec3_uniforms.clear();
    m_ivec4_uniforms.clear();
    m_vec2_uniforms.clear();
    m_vec3_uniforms.clear();
    m_vec4_uniforms.clear();
    m_mat2_uniforms.clear();
    m_mat3_uniforms.clear();
    m_mat4_uniforms.clear();
}

void vup::Shader::update_uniform(const std::string& name, const bool v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_int_uniforms)) {
        m_int_uniforms.at(name).update(static_cast<const int>(v));
    }
}

void vup::Shader::update_uniform(const std::string& name, const unsigned int v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_int_uniforms)) {
        m_int_uniforms.at(name).update(static_cast<int>(v));
    }
}

void vup::Shader::update_uniform(const std::string& name, const int v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_int_uniforms)) {
        m_int_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const float v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_float_uniforms)) {
        m_float_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const double v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_float_uniforms)) {
        m_float_uniforms.at(name).update(static_cast<float>(v));
    }
}

void vup::Shader::update_uniform(const std::string& name, const glm::vec2 v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_vec2_uniforms)) {
        m_vec2_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const glm::vec3 v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_vec3_uniforms)) {
        m_vec3_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const glm::vec4 v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_vec4_uniforms)) {
        m_vec4_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const glm::ivec2 v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_ivec2_uniforms)) {
        m_ivec2_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const glm::ivec3 v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_ivec3_uniforms)) {
        m_ivec3_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const glm::ivec4 v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_ivec4_uniforms)) {
        m_ivec4_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const std::vector<glm::vec2>& v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_vec2_uniforms)) {
        m_vec2_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const std::vector<glm::vec3>& v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_vec3_uniforms)) {
        m_vec3_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const std::vector<glm::vec4>& v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_vec4_uniforms)) {
        m_vec4_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const glm::mat2 v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_mat2_uniforms)) {
        m_mat2_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const glm::mat3 v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_mat3_uniforms)) {
        m_mat3_uniforms.at(name).update(v);
    }
}

void vup::Shader::update_uniform(const std::string& name, const glm::mat4 v) const {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_mat4_uniforms)) {
        m_mat4_uniforms.at(name).update(v);
    }
}

vup::V_F_shader::V_F_shader(filesystem::path vertex_path,
    filesystem::path fragment_path,
    const std::vector<Shader_define>& defines,
    const gl::introspection introspection_flag)
    : Shader(defines, introspection_flag), m_vertex_path(std::move(vertex_path)),
    m_fragment_path(std::move(fragment_path)) {
    std::vector<GLuint> shader_ids(2);
    shader_ids.at(0) = load_shader(m_vertex_path, GL_VERTEX_SHADER);
    shader_ids.at(1) = load_shader(m_fragment_path, GL_FRAGMENT_SHADER);
    init_shader_program(shader_ids);
}

void vup::V_F_shader::reload() {
    clear_maps();
    std::vector<GLuint> shader_ids(2);
    shader_ids.at(0) = load_shader(m_vertex_path, GL_VERTEX_SHADER);
    shader_ids.at(1) = load_shader(m_fragment_path, GL_FRAGMENT_SHADER);
    init_shader_program(shader_ids);
}

vup::V_G_F_shader::V_G_F_shader(filesystem::path vertex_path,
    filesystem::path geometry_path,
    filesystem::path fragment_path,
    const std::vector<Shader_define>& defines,
    const gl::introspection introspection_flag)
    : Shader(defines, introspection_flag), m_vertex_path(std::move(vertex_path)),
    m_geometry_path(std::move(geometry_path)), m_fragment_path(std::move(fragment_path)) {
    std::vector<GLuint> shader_ids(3);
    shader_ids.at(0) = load_shader(m_vertex_path, GL_VERTEX_SHADER);
    shader_ids.at(1) = load_shader(m_geometry_path, GL_GEOMETRY_SHADER);
    shader_ids.at(2) = load_shader(m_fragment_path, GL_FRAGMENT_SHADER);
    init_shader_program(shader_ids);
}

void vup::V_G_F_shader::reload() {
    clear_maps();
    std::vector<GLuint> shader_ids(3);
    shader_ids.at(0) = load_shader(m_vertex_path, GL_VERTEX_SHADER);
    shader_ids.at(1) = load_shader(m_geometry_path, GL_GEOMETRY_SHADER);
    shader_ids.at(2) = load_shader(m_fragment_path, GL_FRAGMENT_SHADER);
    init_shader_program(shader_ids);
}

vup::V_T_F_shader::V_T_F_shader(filesystem::path vertex_path,
    filesystem::path control_path,
    filesystem::path evaluation_path,
    filesystem::path fragment_path,
    const std::vector<Shader_define>& defines,
    const gl::introspection introspection_flag)
    : Shader(defines, introspection_flag), m_vertex_path(std::move(vertex_path)),
    m_control_path(std::move(control_path)), m_evaluation_path(std::move(evaluation_path)),
    m_fragment_path(std::move(fragment_path)) {
    std::vector<GLuint> shader_ids(4);
    shader_ids.at(0) = load_shader(m_vertex_path, GL_VERTEX_SHADER);
    shader_ids.at(1) = load_shader(m_control_path, GL_TESS_CONTROL_SHADER);
    shader_ids.at(2) = load_shader(m_evaluation_path, GL_TESS_EVALUATION_SHADER);
    shader_ids.at(3) = load_shader(m_fragment_path, GL_FRAGMENT_SHADER);
    init_shader_program(shader_ids);
}

void vup::V_T_F_shader::reload() {
    clear_maps();
    std::vector<GLuint> shader_ids(4);
    shader_ids.at(0) = load_shader(m_vertex_path, GL_VERTEX_SHADER);
    shader_ids.at(1) = load_shader(m_control_path, GL_TESS_CONTROL_SHADER);
    shader_ids.at(2) = load_shader(m_evaluation_path, GL_TESS_EVALUATION_SHADER);
    shader_ids.at(3) = load_shader(m_fragment_path, GL_FRAGMENT_SHADER);
    init_shader_program(shader_ids);
}

vup::V_T_G_F_shader::V_T_G_F_shader(filesystem::path vertex_path,
    filesystem::path control_path,
    filesystem::path evaluation_path,
    filesystem::path geometry_path,
    filesystem::path fragment_path,
    const std::vector<Shader_define>& defines,
    const gl::introspection introspection_flag)
    : Shader(defines, introspection_flag), m_vertex_path(std::move(vertex_path)),
    m_control_path(std::move(control_path)), m_evaluation_path(std::move(evaluation_path)),
    m_geometry_path(std::move(geometry_path)), m_fragment_path(std::move(fragment_path)) {
    std::vector<GLuint> shader_ids(5);
    shader_ids.at(0) = load_shader(m_vertex_path, GL_VERTEX_SHADER);
    shader_ids.at(1) = load_shader(m_control_path, GL_TESS_CONTROL_SHADER);
    shader_ids.at(2) = load_shader(m_evaluation_path, GL_TESS_EVALUATION_SHADER);
    shader_ids.at(3) = load_shader(m_geometry_path, GL_GEOMETRY_SHADER);
    shader_ids.at(4) = load_shader(m_fragment_path, GL_FRAGMENT_SHADER);
    init_shader_program(shader_ids);
}

void vup::V_T_G_F_shader::reload() {
    clear_maps();
    std::vector<GLuint> shader_ids(5);
    shader_ids.at(0) = load_shader(m_vertex_path, GL_VERTEX_SHADER);
    shader_ids.at(1) = load_shader(m_control_path, GL_TESS_CONTROL_SHADER);
    shader_ids.at(2) = load_shader(m_evaluation_path, GL_TESS_EVALUATION_SHADER);
    shader_ids.at(3) = load_shader(m_geometry_path, GL_GEOMETRY_SHADER);
    shader_ids.at(4) = load_shader(m_fragment_path, GL_FRAGMENT_SHADER);
    init_shader_program(shader_ids);
}
