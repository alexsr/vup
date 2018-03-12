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
                                gl::introspection introspection_flag = gl::introspection::basic,
                                const std::vector<Shader_define>& defines = {});
        void reload() override;
        void run(float x = 1, float y = 1, float z = 1) const;
        void run_with_barrier(float x = 1, float y = 1, float z = 1,
                              GLbitfield barriers = GL_SHADER_STORAGE_BARRIER_BIT) const;
        void run_workgroups(GLuint x = 1, GLuint y = 1, GLuint z = 1) const;
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
