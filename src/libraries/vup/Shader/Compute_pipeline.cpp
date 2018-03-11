//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Compute_pipeline.h"

vup::Compute_pipeline::Compute_pipeline(const std::vector<filesystem::path>& shader_paths,
                                        gl::introspection introspection_flag,
                                        const std::vector<Shader_define>& defines,
                                        const filesystem::path& main_dir_path) {
    for (const auto& p : shader_paths) {
        m_compute_shaders.emplace_back(main_dir_path / p, introspection_flag, defines);;
    }
}

void vup::Compute_pipeline::reload() {
    for (auto& c : m_compute_shaders) {
        c.reload();
    }
}

void vup::Compute_pipeline::run(const float x, const float y, const float z) const {
    for (const auto& c : m_compute_shaders) {
        c.run(x, y, z);
    }
}

void vup::Compute_pipeline::run_with_barrier(const float x, const float y, const float z,
                                             const GLbitfield barriers) const {
    for (const auto& c : m_compute_shaders) {
        c.run_with_barrier(x, y, z, barriers);
    }
}

void vup::Compute_pipeline::run_workgroups(const GLuint x, const GLuint y, const GLuint z) const {
    for (const auto& c : m_compute_shaders) {
        c.run_workgroups(x, y, z);
    }
}
