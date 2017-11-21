//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "V_T_G_F_shader_program.h"

vup::V_T_G_F_shader_program::V_T_G_F_shader_program(
        const std::shared_ptr<vup::Vertex_shader>& vertex,
        const std::shared_ptr<vup::Control_shader>& control,
        const std::shared_ptr<vup::Evaluation_shader>& evaluation,
        const std::shared_ptr<vup::Geometry_shader>& geometry,
        const std::shared_ptr<vup::Fragment_shader>& fragment,
        vup::gl::introspection introspection_flag)
        : Shader_program(introspection_flag) {
    m_vertex = vertex;
    m_control = control;
    m_evaluation = evaluation;
    m_geometry = geometry;
    m_fragment = fragment;
    init_shader_program();
}

void vup::V_T_G_F_shader_program::reload() {
    m_vertex->reload();
    m_control->reload();
    m_evaluation->reload();
    m_geometry->reload();
    m_fragment->reload();
    clear_maps();
    init_shader_program();
}

void vup::V_T_G_F_shader_program::attach_shaders() const {
    glAttachShader(m_program_id, m_vertex->get_id());
    glAttachShader(m_program_id, m_control->get_id());
    glAttachShader(m_program_id, m_evaluation->get_id());
    glAttachShader(m_program_id, m_geometry->get_id());
    glAttachShader(m_program_id, m_fragment->get_id());
}

void vup::V_T_G_F_shader_program::detach_shaders() const {
    glDetachShader(m_program_id, m_vertex->get_id());
    glDetachShader(m_program_id, m_control->get_id());
    glDetachShader(m_program_id, m_evaluation->get_id());
    glDetachShader(m_program_id, m_geometry->get_id());
    glDetachShader(m_program_id, m_fragment->get_id());
}
