//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Compute_shader_program.h"

vup::Compute_shader_program::Compute_shader_program(const std::shared_ptr<vup::Compute_shader>& compute,
                                                    vup::gl::Introspection introspection_flag)
        : Shader_program(introspection_flag) {
    m_compute = compute;
    init_shader_program();
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

void vup::Compute_shader_program::reload() {
    m_compute->reload();
    clear_maps();
    init_shader_program();
    glGetProgramiv(m_program_id, GL_COMPUTE_WORK_GROUP_SIZE, m_workgroup_size.data());
}

void vup::Compute_shader_program::attach_shaders() const {
    glAttachShader(m_program_id, m_compute->get_id());
}

void vup::Compute_shader_program::detach_shaders() const {
    glDetachShader(m_program_id, m_compute->get_id());
}

void vup::Compute_shader_program::run(float x, float y, float z) {
    use();
    glDispatchCompute(static_cast<GLuint>(glm::ceil(x / m_workgroup_size.at(0))),
                      static_cast<GLuint>(glm::ceil(y / m_workgroup_size.at(1))),
                      static_cast<GLuint>(glm::ceil(z / m_workgroup_size.at(2))));
}

void vup::Compute_shader_program::run_workgroups(GLuint x, GLuint y, GLuint z) {
    use();
    glDispatchCompute(x, y, z);
}
