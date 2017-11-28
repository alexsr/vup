//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//
#include "V_F_shader.h"

vup::V_F_shader::V_F_shader(const filesystem::path& vertex_path,
                                            const filesystem::path& fragment_path,
                                            vup::gl::Introspection introspection_flag)
        : Shader(introspection_flag), m_vertex_path(vertex_path), m_fragment_path(fragment_path) {
    std::vector<GLuint> shader_ids;
    shader_ids.push_back(load_shader(m_vertex_path, GL_VERTEX_SHADER));
    shader_ids.push_back(load_shader(m_fragment_path, GL_FRAGMENT_SHADER));
    init_shader_program(shader_ids);
}

void vup::V_F_shader::reload() {
    clear_maps();
    std::vector<GLuint> shader_ids;
    shader_ids.push_back(load_shader(m_vertex_path, GL_VERTEX_SHADER));
    shader_ids.push_back(load_shader(m_fragment_path, GL_FRAGMENT_SHADER));
    init_shader_program(shader_ids);
}
