//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_COMPUTE_SHADER_PROGRAM_H
#define VUP_COMPUTE_SHADER_PROGRAM_H

#include "Shader.h"

namespace vup
{
    class Compute_shader : public Shader {
    public:
        explicit Compute_shader(filesystem::path compute_path,
                                const std::vector<Shader_define>& defines = {},
                                gl::introspection introspection_flag = gl::introspection::basic);
        void reload() override;
        void run(unsigned int x = 1, unsigned int y = 1, unsigned int z = 1) const;
        void run_with_barrier(unsigned int x = 1, unsigned int y = 1, unsigned int z = 1,
                              GLbitfield barriers = GL_SHADER_STORAGE_BARRIER_BIT) const;
        void run_workgroups(unsigned int x = 1, unsigned int y = 1, unsigned int z = 1) const;
        void run_workgroups_with_barrier(unsigned int x = 1, unsigned int y = 1, unsigned int z = 1,
                                         GLbitfield barriers = GL_SHADER_STORAGE_BARRIER_BIT) const;
        std::array<GLint, 3> get_workgroup_size() const;
        GLint get_workgroup_size_x() const;
        GLint get_workgroup_size_y() const;
        GLint get_workgroup_size_z() const;
    private:
        filesystem::path m_compute_path;
        std::array<GLint, 3> m_workgroup_size{};
    };
}

#endif //VUP_COMPUTE_SHADER_PROGRAM_H
