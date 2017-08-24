//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//
#include "V_F_shader_program.h"

vup::V_F_shader_program::V_F_shader_program(const std::shared_ptr<vup::Shader>& vertex,
                                            const std::shared_ptr<vup::Shader>& fragment) {
    m_vertex = vertex;
    m_fragment = fragment;
    attach_shaders();
    link_program();
}

void vup::V_F_shader_program::reload() {
    m_vertex->reload();
    m_fragment->reload();
    attach_shaders();
    link_program();
}

void vup::V_F_shader_program::attach_shaders() {
    glAttachShader(m_program_id, m_vertex->get_id());
    glAttachShader(m_program_id, m_fragment->get_id());
}
