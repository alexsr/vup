//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_COMPUTE_PIPELINE_H
#define VUP_COMPUTE_PIPELINE_H

#include "Compute_shader.h"

namespace vup
{
    class Compute_pipeline {
    public:
        explicit Compute_pipeline(const std::vector<filesystem::path>& shader_paths,
                                  const std::vector<Shader_define>& defines = {},
                                  const filesystem::path& main_dir_path = "",
                                  gl::introspection introspection_flag = gl::introspection::basic);
        void reload();
        void run(unsigned int x = 1, unsigned int y = 1, unsigned int z = 1) const;
        void run_with_barrier(unsigned int x = 1, unsigned int y = 1, unsigned int z = 1,
                              GLbitfield barriers = GL_SHADER_STORAGE_BARRIER_BIT) const;
        void run_workgroups(unsigned int x = 1, unsigned int y = 1, unsigned int z = 1) const;
        void run_workgroups_with_barrier(unsigned int x = 1, unsigned int y = 1, unsigned int z = 1,
                                         GLbitfield barriers = GL_SHADER_STORAGE_BARRIER_BIT) const;
        template <typename T>
        void update_uniform(const std::string& name, T v) const;
        template <typename T>
        void update_uniform_at(unsigned int id, const std::string& name, T v) const;
    private:
        std::vector<Compute_shader> m_compute_shaders;
    };

    template <typename T>
    void Compute_pipeline::update_uniform(const std::string& name, T v) const {
        for (const auto& c : m_compute_shaders) {
            c.update_uniform(name, v);
        }
    }

    template <typename T>
    void Compute_pipeline::update_uniform_at(const unsigned int id, const std::string& name, T v) const {
        m_compute_shaders.at(id).update_uniform(name, v);
    }
}

#endif //VUP_COMPUTE_PIPELINE_H
