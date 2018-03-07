//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//
#include "V_F_shader.h"
#include <utility>

vup::V_F_shader::V_F_shader(filesystem::path vertex_path,
                            filesystem::path fragment_path,
                            const gl::introspection introspection_flag,
                            const std::vector<Shader_define>& defines)
    : Shader(introspection_flag, defines), m_vertex_path(std::move(vertex_path)),
      m_fragment_path(std::move(fragment_path)) {
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
