//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "V_T_G_F_shader.h"

vup::V_T_G_F_shader::V_T_G_F_shader(const filesystem::path& vertex_path,
                                    const filesystem::path& control_path,
                                    const filesystem::path& evaluation_path,
                                    const filesystem::path& geometry_path,
                                    const filesystem::path& fragment_path,
                                    vup::gl::introspection introspection_flag,
                                    const std::vector<Shader_define>& defines)
    : Shader(introspection_flag, defines), m_vertex_path(vertex_path),
      m_control_path(control_path), m_evaluation_path(evaluation_path),
      m_geometry_path(geometry_path), m_fragment_path(fragment_path) {
    std::vector<GLuint> shader_ids;
    shader_ids.push_back(load_shader(m_vertex_path, GL_VERTEX_SHADER));
    shader_ids.push_back(load_shader(m_control_path, GL_TESS_CONTROL_SHADER));
    shader_ids.push_back(load_shader(m_evaluation_path, GL_TESS_EVALUATION_SHADER));
    shader_ids.push_back(load_shader(m_geometry_path, GL_GEOMETRY_SHADER));
    shader_ids.push_back(load_shader(m_fragment_path, GL_FRAGMENT_SHADER));
    init_shader_program(shader_ids);
}

void vup::V_T_G_F_shader::reload() {
    clear_maps();
    std::vector<GLuint> shader_ids;
    shader_ids.push_back(load_shader(m_vertex_path, GL_VERTEX_SHADER));
    shader_ids.push_back(load_shader(m_control_path, GL_TESS_CONTROL_SHADER));
    shader_ids.push_back(load_shader(m_evaluation_path, GL_TESS_EVALUATION_SHADER));
    shader_ids.push_back(load_shader(m_geometry_path, GL_GEOMETRY_SHADER));
    shader_ids.push_back(load_shader(m_fragment_path, GL_FRAGMENT_SHADER));
    init_shader_program(shader_ids);
}
