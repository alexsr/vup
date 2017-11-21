//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Shader_program.h"

vup::Shader_program::Shader_program(vup::gl::introspection introspection_flag)
        : m_introspection_flag(introspection_flag) {
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
                                 + vup::gl::shader_type_to_string(m_program_id) +
                                 ".\n"
                                 + "Error log: \n"
                                 + error_log};
    }
}

void vup::Shader_program::init_shader_program() {
    attach_shaders();
    link_program();
    detach_shaders();
    if (m_introspection_flag != vup::gl::introspection::none) {
        inspect_uniforms();
        if (m_introspection_flag & vup::gl::introspection::ubos) {
            inspect_uniform_blocks();
        }
        if (m_introspection_flag & vup::gl::introspection::ssbos) {
            inspect_shader_storage_blocks();
        }
    }
}

void vup::Shader_program::inspect_uniforms() {
    GLint uniform_count = 0;
    glGetProgramInterfaceiv(m_program_id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniform_count);
    std::array<GLenum, 4> properties{GL_BLOCK_INDEX, GL_TYPE, GL_NAME_LENGTH, GL_LOCATION};
    for (unsigned int i = 0; i < uniform_count; i++) {
        std::array<GLint, 4> info{};
        glGetProgramResourceiv(m_program_id, GL_UNIFORM, i, 4, properties.data(), 4, nullptr, info.data());
        if (info.at(0) != -1) {
            continue; // skip block uniforms
        }
        std::vector<char> name_data(static_cast<unsigned long>(info.at(2)));
        glGetProgramResourceName(m_program_id, GL_UNIFORM, i, static_cast<GLsizei>(name_data.size()),
                                 nullptr, name_data.data());
        std::string name(name_data.begin(), name_data.end()-1);
        add_uniform(name, info.at(1), info.at(3));
    }
}

void vup::Shader_program::add_uniform(const std::string& name, GLint type,
                                      GLint location) {
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

void vup::Shader_program::inspect_uniform_blocks() {
    GLint buffer_block_count = 0;
    glGetProgramInterfaceiv(m_program_id, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &buffer_block_count);
    std::array<GLenum, 3> properties = {GL_BUFFER_BINDING, GL_NAME_LENGTH, GL_BUFFER_DATA_SIZE};
    for (unsigned int i = 0; i < buffer_block_count; i++) {
        std::array<GLint, 3> info = {};
        glGetProgramResourceiv(m_program_id, GL_UNIFORM_BLOCK, i, 3, properties.data(), 3, nullptr, info.data());
        std::vector<char> name_data(static_cast<unsigned long>(info.at(1)));
        glGetProgramResourceName(m_program_id, GL_UNIFORM_BLOCK, i, static_cast<GLsizei>(name_data.size()),
                                 nullptr, name_data.data());
        std::string name(name_data.begin(), name_data.end()-1);
        m_ubos.emplace(std::piecewise_construct, std::make_tuple(name),
                  std::make_tuple(static_cast<GLuint>(info.at(0)), static_cast<unsigned int>(info.at(2))));
    }
}

void vup::Shader_program::inspect_shader_storage_blocks() {
    GLint buffer_block_count = 0;
    glGetProgramInterfaceiv(m_program_id, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &buffer_block_count);
    std::array<GLenum, 3> properties = {GL_BUFFER_BINDING, GL_NAME_LENGTH, GL_BUFFER_DATA_SIZE};
    for (unsigned int i = 0; i < buffer_block_count; i++) {
        std::array<GLint, 3> info = {};
        glGetProgramResourceiv(m_program_id, GL_SHADER_STORAGE_BLOCK, i, 3, properties.data(), 3, nullptr, info.data());
        std::vector<char> name_data(static_cast<unsigned long>(info.at(1)));
        glGetProgramResourceName(m_program_id, GL_SHADER_STORAGE_BLOCK, i, static_cast<GLsizei>(name_data.size()),
                                 nullptr, name_data.data());
        std::string name(name_data.begin(), name_data.end()-1);
        m_ssbos.emplace(std::piecewise_construct, std::make_tuple(name),
                       std::make_tuple(static_cast<GLuint>(info.at(0))));
    }
}

void vup::Shader_program::clear_maps() {
    m_ubos.clear();
    m_ssbos.clear();
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

void vup::Shader_program::update_uniform(const std::string& name, bool v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_int_uniforms)) {
        m_int_uniforms.at(name).update({static_cast<const int>(v)});
    }
}

void vup::Shader_program::update_uniform(const std::string& name, int v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_int_uniforms)) {
        m_int_uniforms.at(name).update({v});
    }
}

void vup::Shader_program::update_uniform(const std::string& name, float v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_float_uniforms)) {
        m_float_uniforms.at(name).update({v});
    }
}

void vup::Shader_program::update_uniform(const std::string& name, double v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_float_uniforms)) {
        m_float_uniforms.at(name).update({static_cast<float>(v)});
    }
}

void vup::Shader_program::update_uniform(const std::string& name, glm::vec2 v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_vec2_uniforms)) {
        m_vec2_uniforms.at(name).update({v});
    }
}

void vup::Shader_program::update_uniform(const std::string& name, glm::vec3 v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_vec3_uniforms)) {
        m_vec3_uniforms.at(name).update({v});
    }
}

void vup::Shader_program::update_uniform(const std::string& name, glm::vec4 v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_vec4_uniforms)) {
        m_vec4_uniforms.at(name).update({v});
    }
}

void vup::Shader_program::update_uniform(const std::string& name, glm::ivec2 v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_ivec2_uniforms)) {
        m_ivec2_uniforms.at(name).update({v});
    }
}

void vup::Shader_program::update_uniform(const std::string& name, glm::ivec3 v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_ivec3_uniforms)) {
        m_ivec3_uniforms.at(name).update({v});
    }
}

void vup::Shader_program::update_uniform(const std::string& name, glm::ivec4 v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_ivec4_uniforms)) {
        m_ivec4_uniforms.at(name).update({v});
    }
}

void vup::Shader_program::update_uniform(const std::string& name, std::vector<glm::vec2> v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_vec2_uniforms)) {
        m_vec2_uniforms.at(name).update(v);
    }
}

void vup::Shader_program::update_uniform(const std::string& name, std::vector<glm::vec3> v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_vec3_uniforms)) {
        m_vec3_uniforms.at(name).update(v);
    }
}

void vup::Shader_program::update_uniform(const std::string& name, std::vector<glm::vec4> v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_vec4_uniforms)) {
        m_vec4_uniforms.at(name).update(v);
    }
}

void vup::Shader_program::update_uniform(const std::string& name, glm::mat2 v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_mat2_uniforms)) {
        m_mat2_uniforms.at(name).update({v});
    }
}

void vup::Shader_program::update_uniform(const std::string& name, glm::mat3 v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_mat3_uniforms)) {
        m_mat3_uniforms.at(name).update({v});
    }
}

void vup::Shader_program::update_uniform(const std::string& name, glm::mat4 v) {
    glUseProgram(m_program_id);
    if (find_map_entry(name, m_mat4_uniforms)) {
        m_mat4_uniforms.at(name).update({v});
    }
}
