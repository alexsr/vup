//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "V_G_F_shader_program.h"

vup::V_G_F_shader_program::V_G_F_shader_program(
        const std::shared_ptr<vup::Shader>& vertex,
        const std::shared_ptr<vup::Shader>& geometry,
        const std::shared_ptr<vup::Shader>& fragment) {
    m_vertex = vertex;
    m_geometry = geometry;
    m_fragment = fragment;
    attach_shaders();
    link_program();
}

void vup::V_G_F_shader_program::attach_shaders() {
    glAttachShader(m_program_id, m_vertex->get_id());
    glAttachShader(m_program_id, m_geometry->get_id());
    glAttachShader(m_program_id, m_fragment->get_id());
}
