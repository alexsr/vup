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
    class Compute_shader : public vup::Shader {
    public:
        explicit Compute_shader(const filesystem::path& compute_path,
                                vup::gl::Introspection introspection_flag = vup::gl::Introspection::basic,
                                const std::vector<Shader_define>& defines = {});
        void reload() override;
        void run(float x = 1, float y = 1, float z = 1);
        void run_workgroups(GLuint x = 1, GLuint y = 1, GLuint z = 1);
        std::array<GLint, 3> get_workgroup_size();
        GLint get_workgroup_size_x();
        GLint get_workgroup_size_y();
        GLint get_workgroup_size_z();
    private:
        filesystem::path m_compute_path;
        std::array<GLint, 3> m_max_workgroup_size;
        std::array<GLint, 3> m_max_workgroup_count;
        std::array<GLint, 3> m_workgroup_size;
        GLint m_max_workgroup_count_invocations = 0;
        GLint m_max_shared_memory_size = 0;
    };
}

#endif //VUP_COMPUTE_SHADER_PROGRAM_H
