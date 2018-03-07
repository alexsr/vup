//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_V_F_SHADER_PROGRAM_H
#define VUP_V_F_SHADER_PROGRAM_H

#include "Shader.h"

namespace vup
{
    class V_F_shader : public Shader {
    public:
        V_F_shader(filesystem::path vertex_path,
                   filesystem::path fragment_path,
                   gl::introspection introspection_flag = gl::introspection::basic,
                   const std::vector<Shader_define>& defines = {});
        virtual ~V_F_shader() = default;
        void reload() override;
    private:
        filesystem::path m_vertex_path;
        filesystem::path m_fragment_path;
    };
}


#endif //VUP_V_F_SHADER_PROGRAM_H
