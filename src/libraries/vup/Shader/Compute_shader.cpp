//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Compute_shader.h"

vup::Compute_shader::Compute_shader(const filesystem::path& compute_path,
                                    vup::gl::introspection introspection_flag,
                                    const std::vector<Shader_define>& defines)
    : Shader(introspection_flag, defines), m_compute_path(compute_path) {
    std::vector<GLuint> shader_ids;
    shader_ids.push_back(load_shader(m_compute_path, GL_COMPUTE_SHADER));
    init_shader_program(shader_ids);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &m_max_workgroup_count.at(0));
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &m_max_workgroup_count.at(1));
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &m_max_workgroup_count.at(2));
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &m_max_workgroup_size.at(0));
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &m_max_workgroup_size.at(1));
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &m_max_workgroup_size.at(2));
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &m_max_workgroup_count_invocations);
    glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &m_max_shared_memory_size);
    glGetProgramiv(m_program_id, GL_COMPUTE_WORK_GROUP_SIZE, m_workgroup_size.data());
}

void vup::Compute_shader::reload() {
    clear_maps();
    std::vector<GLuint> shader_ids;
    shader_ids.push_back(load_shader(m_compute_path, GL_COMPUTE_SHADER));
    init_shader_program(shader_ids);
    glGetProgramiv(m_program_id, GL_COMPUTE_WORK_GROUP_SIZE, m_workgroup_size.data());
}

void vup::Compute_shader::run(float x, float y, float z) {
    use();
    glDispatchCompute(static_cast<GLuint>(glm::ceil(x / m_workgroup_size.at(0))),
                      static_cast<GLuint>(glm::ceil(y / m_workgroup_size.at(1))),
                      static_cast<GLuint>(glm::ceil(z / m_workgroup_size.at(2))));
}

void vup::Compute_shader::run_with_barrier(float x, float y, float z, GLbitfield barriers) {
    use();
    glDispatchCompute(static_cast<GLuint>(glm::ceil(x / m_workgroup_size.at(0))),
                      static_cast<GLuint>(glm::ceil(y / m_workgroup_size.at(1))),
                      static_cast<GLuint>(glm::ceil(z / m_workgroup_size.at(2))));
    glMemoryBarrier(barriers);
}

void vup::Compute_shader::run_workgroups(GLuint x, GLuint y, GLuint z) {
    use();
    glDispatchCompute(x, y, z);
}

std::array<GLint, 3> vup::Compute_shader::get_workgroup_size() {
    return m_workgroup_size;
}

GLint vup::Compute_shader::get_workgroup_size_x() {
    return m_workgroup_size.at(0);
}

GLint vup::Compute_shader::get_workgroup_size_y() {
    return m_workgroup_size.at(1);
}

GLint vup::Compute_shader::get_workgroup_size_z() {
    return m_workgroup_size.at(2);
}
