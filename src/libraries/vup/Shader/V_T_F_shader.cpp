//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "V_T_F_shader.h"

vup::V_T_F_shader::V_T_F_shader(filesystem::path vertex_path,
                                filesystem::path control_path,
                                filesystem::path evaluation_path,
                                filesystem::path fragment_path,
                                const gl::introspection introspection_flag,
                                const std::vector<Shader_define>& defines)
    : Shader(introspection_flag, defines), m_vertex_path(std::move(vertex_path)),
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
