//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "V_G_F_shader.h"

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
