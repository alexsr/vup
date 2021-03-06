//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Compute_pipeline.h"

vup::Compute_pipeline::Compute_pipeline(const std::vector<filesystem::path>& shader_paths,
                                        const std::vector<Shader_define>& defines,
                                        const filesystem::path& main_dir_path,
                                        gl::introspection introspection_flag) {
    for (const auto& p : shader_paths) {
        m_compute_shaders.emplace_back(main_dir_path / p, defines, introspection_flag);;
    }
}

void vup::Compute_pipeline::reload() {
    for (auto& c : m_compute_shaders) {
        c.reload();
    }
}

void vup::Compute_pipeline::run(const unsigned int x, const unsigned int y, const unsigned int z) const {
    for (const auto& c : m_compute_shaders) {
        c.run(x, y, z);
    }
}

void vup::Compute_pipeline::run_with_barrier(const unsigned x, const unsigned y, const unsigned z,
                                             const GLbitfield barriers) const {
    for (const auto& c : m_compute_shaders) {
        c.run_with_barrier(x, y, z, barriers);
    }
}

void vup::Compute_pipeline::run_workgroups(const unsigned x, const unsigned y, const unsigned z) const {
    for (const auto& c : m_compute_shaders) {
        c.run_workgroups(x, y, z);
    }
}

void vup::Compute_pipeline::run_workgroups_with_barrier(const unsigned x, const unsigned y, const unsigned z,
                                                        const GLbitfield barriers) const {
    for (const auto& c : m_compute_shaders) {
        c.run_workgroups_with_barrier(x, y, z, barriers);
    }
}
