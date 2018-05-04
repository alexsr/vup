//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Compute_shader.h"
#include <utility>

vup::Compute_shader::Compute_shader(filesystem::path compute_path,
                                    const std::vector<Shader_define>& defines,
                                    const gl::introspection introspection_flag)
    : Shader(defines, introspection_flag), m_compute_path(std::move(compute_path)) {
    std::vector<GLuint> shader_ids;
    shader_ids.push_back(load_shader(m_compute_path, GL_COMPUTE_SHADER));
    init_shader_program(shader_ids);
    glGetProgramiv(m_program_id, GL_COMPUTE_WORK_GROUP_SIZE, m_workgroup_size.data());
}

void vup::Compute_shader::reload() {
    clear_maps();
    std::vector<GLuint> shader_ids;
    shader_ids.push_back(load_shader(m_compute_path, GL_COMPUTE_SHADER));
    init_shader_program(shader_ids);
    glGetProgramiv(m_program_id, GL_COMPUTE_WORK_GROUP_SIZE, m_workgroup_size.data());
}

void vup::Compute_shader::run(const unsigned int x, const unsigned int y, const unsigned int z) const {
    use();
    glDispatchCompute(static_cast<GLuint>(glm::ceil(static_cast<float>(x) / m_workgroup_size.at(0))),
                      static_cast<GLuint>(glm::ceil(static_cast<float>(y) / m_workgroup_size.at(1))),
                      static_cast<GLuint>(glm::ceil(static_cast<float>(z) / m_workgroup_size.at(2))));
}

void vup::Compute_shader::run_with_barrier(const unsigned int x, const unsigned int y, const unsigned int z,
                                           const GLbitfield barriers) const {
    use();
    glDispatchCompute(static_cast<GLuint>(glm::ceil(static_cast<float>(x) / m_workgroup_size.at(0))),
                      static_cast<GLuint>(glm::ceil(static_cast<float>(y) / m_workgroup_size.at(1))),
                      static_cast<GLuint>(glm::ceil(static_cast<float>(z) / m_workgroup_size.at(2))));
    glMemoryBarrier(barriers);
}

void vup::Compute_shader::run_workgroups(const unsigned int x, const unsigned int y, const unsigned int z) const {
    use();
    glDispatchCompute(x, y, z);
}

std::array<GLint, 3> vup::Compute_shader::get_workgroup_size() const {
    return m_workgroup_size;
}

GLint vup::Compute_shader::get_workgroup_size_x() const {
    return m_workgroup_size.at(0);
}

GLint vup::Compute_shader::get_workgroup_size_y() const {
    return m_workgroup_size.at(1);
}

GLint vup::Compute_shader::get_workgroup_size_z() const {
    return m_workgroup_size.at(2);
}
