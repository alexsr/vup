//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//
#include "V_F_shader_program.h"

vup::V_F_shader_program::V_F_shader_program(const std::shared_ptr<vup::Vertex_shader>& vertex,
                                            const std::shared_ptr<vup::Fragment_shader>& fragment) {
    m_vertex = vertex;
    m_fragment = fragment;
    attach_shaders();
    link_program();
    detach_shaders();
    analyze_uniforms();
    analyze_uniform_blocks();
}

void vup::V_F_shader_program::reload() {
    m_vertex->reload();
    m_fragment->reload();
    attach_shaders();
    link_program();
    detach_shaders();
    analyze_uniforms();
    analyze_uniform_blocks();
}

void vup::V_F_shader_program::attach_shaders() const {
    glAttachShader(m_program_id, m_vertex->get_id());
    glAttachShader(m_program_id, m_fragment->get_id());
}

void vup::V_F_shader_program::detach_shaders() const {
    glDetachShader(m_program_id, m_vertex->get_id());
    glDetachShader(m_program_id, m_fragment->get_id());
}
