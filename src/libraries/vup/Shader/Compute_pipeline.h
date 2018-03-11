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
                                  gl::introspection introspection_flag = gl::introspection::basic,
                                  const std::vector<Shader_define>& defines = {},
                                  const filesystem::path& main_dir_path = "");
        void reload();
        void run(float x = 1, float y = 1, float z = 1);
        void run_with_barrier(float x = 1, float y = 1, float z = 1,
                              GLbitfield barriers = GL_SHADER_STORAGE_BARRIER_BIT);
        void run_workgroups(GLuint x = 1, GLuint y = 1, GLuint z = 1);
        template <typename T>
        void update_uniform(const std::string& name, T v);
        template <typename T>
        void update_uniform_at(unsigned int id, const std::string& name, T v);
    private:
        std::vector<Compute_shader> m_compute_shaders;
    };

    template <typename T>
    void Compute_pipeline::update_uniform(const std::string& name, T v) {
        for (auto& c : m_compute_shaders) {
            c.update_uniform(name, v);
        }
    }

    template <typename T>
    void Compute_pipeline::update_uniform_at(unsigned int id, const std::string& name, T v) {
        m_compute_shaders.at(id).update_uniform(name, v);
    }
}

#endif //VUP_COMPUTE_PIPELINE_H
