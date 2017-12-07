//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_V_T_F_SHADER_PROGRAM_H
#define VUP_V_T_F_SHADER_PROGRAM_H

#include "Shader.h"

namespace vup
{
    class V_T_F_shader : public vup::Shader {
    public:
        V_T_F_shader(const filesystem::path& vertex_path,
                     const filesystem::path& control_path,
                     const filesystem::path& evaluation_path,
                     const filesystem::path& fragment_path,
                     vup::gl::Introspection introspection_flag = vup::gl::Introspection::basic,
                     const std::vector<Shader_define>& defines = {});
        void reload() override;
    private:
        filesystem::path m_vertex_path;
        filesystem::path m_control_path;
        filesystem::path m_evaluation_path;
        filesystem::path m_fragment_path;
    };
}

#endif //VUP_V_T_F_SHADER_PROGRAM_H
