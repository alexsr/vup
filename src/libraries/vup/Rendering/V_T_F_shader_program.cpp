//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "V_T_F_shader_program.h"

vup::V_T_F_shader_program::V_T_F_shader_program(
        const std::shared_ptr<vup::Vertex_shader>& vertex,
        const std::shared_ptr<vup::Control_shader>& control,
        const std::shared_ptr<vup::Evaluation_shader>& evaluation,
        const std::shared_ptr<vup::Fragment_shader>& fragment) {
    m_vertex = vertex;
    m_control = control;
    m_evaluation = evaluation;
    m_fragment = fragment;
    attach_shaders();
    link_program();
    detach_shaders();
}

void vup::V_T_F_shader_program::reload() {
    m_vertex->reload();
    m_control->reload();
    m_evaluation->reload();
    m_fragment->reload();
    attach_shaders();
    link_program();
    detach_shaders();
}

void vup::V_T_F_shader_program::attach_shaders() {
    glAttachShader(m_program_id, m_vertex->get_id());
    glAttachShader(m_program_id, m_control->get_id());
    glAttachShader(m_program_id, m_evaluation->get_id());
    glAttachShader(m_program_id, m_fragment->get_id());
}

void vup::V_T_F_shader_program::detach_shaders() {
    glDetachShader(m_program_id, m_vertex->get_id());
    glDetachShader(m_program_id, m_control->get_id());
    glDetachShader(m_program_id, m_evaluation->get_id());
    glDetachShader(m_program_id, m_fragment->get_id());
}
