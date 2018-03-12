//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_V_T_G_F_SHADER_PROGRAM_H
#define VUP_V_T_G_F_SHADER_PROGRAM_H

#include "Shader.h"

namespace vup
{
    class V_T_G_F_shader : public Shader {
    public:
        V_T_G_F_shader(filesystem::path vertex_path,
                       filesystem::path control_path,
                       filesystem::path evaluation_path,
                       filesystem::path geometry_path,
                       filesystem::path fragment_path,
                       gl::introspection introspection_flag = gl::introspection::basic,
                       const std::vector<Shader_define>& defines = {});
        void reload() override;
    private:
        filesystem::path m_vertex_path;
        filesystem::path m_control_path;
        filesystem::path m_evaluation_path;
        filesystem::path m_geometry_path;
        filesystem::path m_fragment_path;
    };
}

#endif //VUP_V_T_G_F_SHADER_PROGRAM_H
