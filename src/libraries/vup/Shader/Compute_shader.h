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
        explicit Compute_shader(const filesystem::path& compute_path,
                                gl::introspection introspection_flag = gl::introspection::basic,
                                const std::vector<Shader_define>& defines = {});
        void reload() override;
        void run(float x = 1, float y = 1, float z = 1);
        void run_with_barrier(float x = 1, float y = 1, float z = 1,
                              GLbitfield barriers = GL_SHADER_STORAGE_BARRIER_BIT);
        void run_workgroups(GLuint x = 1, GLuint y = 1, GLuint z = 1);
        std::array<GLint, 3> get_workgroup_size();
        GLint get_workgroup_size_x();
        GLint get_workgroup_size_y();
        GLint get_workgroup_size_z();
    private:
        filesystem::path m_compute_path;
        std::array<GLint, 3> m_workgroup_size{};
    };
}

#endif //VUP_COMPUTE_SHADER_PROGRAM_H
