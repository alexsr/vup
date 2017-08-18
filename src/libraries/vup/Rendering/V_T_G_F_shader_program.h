//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_V_T_G_F_SHADER_PROGRAM_H
#define VUP_V_T_G_F_SHADER_PROGRAM_H

#include "Shader_program.h"

namespace vup
{
    class V_T_G_F_shader_program : public vup::Shader_program {
    public:
        V_T_G_F_shader_program(const std::shared_ptr<vup::Shader>& vertex,
                               const std::shared_ptr<vup::Shader>& control,
                               const std::shared_ptr<vup::Shader>& evaluation,
                             const std::shared_ptr<vup::Shader>& geometry,
                             const std::shared_ptr<vup::Shader>& fragment);
    protected:
        void attach_shaders() override;
    private:
        std::shared_ptr<vup::Shader> m_vertex;
        std::shared_ptr<vup::Shader> m_control;
        std::shared_ptr<vup::Shader> m_evaluation;
        std::shared_ptr<vup::Shader> m_geometry;
        std::shared_ptr<vup::Shader> m_fragment;
    };
}

#endif //VUP_V_T_G_F_SHADER_PROGRAM_H
