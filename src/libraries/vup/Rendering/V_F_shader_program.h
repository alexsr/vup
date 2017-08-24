//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_V_F_SHADER_PROGRAM_H
#define VUP_V_F_SHADER_PROGRAM_H

#include "Shader_program.h"

namespace vup
{
    class V_F_shader_program : vup::Shader_program {
    public:
        V_F_shader_program(const std::shared_ptr<vup::Shader>& vertex,
                           const std::shared_ptr<vup::Shader>& fragment);
        void reload() override;
    protected:
        void attach_shaders() override;
        void detach_shaders() override;
    private:
        std::shared_ptr<vup::Shader> m_vertex;
        std::shared_ptr<vup::Shader> m_fragment;
    };
}


#endif //VUP_V_F_SHADER_PROGRAM_H