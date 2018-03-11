//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Compute_shader.h"

vup::Compute_shader::Compute_shader(const filesystem::path& compute_path,
                                    gl::introspection introspection_flag,
                                    const std::vector<Shader_define>& defines)
    : Shader(introspection_flag, defines), m_compute_path(compute_path) {
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

void vup::Compute_shader::run(const float x, const float y, const float z) const {
    use();
    glDispatchCompute(static_cast<GLuint>(glm::ceil(x / m_workgroup_size.at(0))),
                      static_cast<GLuint>(glm::ceil(y / m_workgroup_size.at(1))),
                      static_cast<GLuint>(glm::ceil(z / m_workgroup_size.at(2))));
}

void vup::Compute_shader::run_with_barrier(const float x, const float y, const float z,
                                           const GLbitfield barriers) const {
    use();
    glDispatchCompute(static_cast<GLuint>(glm::ceil(x / m_workgroup_size.at(0))),
                      static_cast<GLuint>(glm::ceil(y / m_workgroup_size.at(1))),
                      static_cast<GLuint>(glm::ceil(z / m_workgroup_size.at(2))));
    glMemoryBarrier(barriers);
}

void vup::Compute_shader::run_workgroups(const GLuint x, const GLuint y, const GLuint z) const {
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
