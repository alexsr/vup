//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_V_G_F_SHADER_PROGRAM_H
#define VUP_V_G_F_SHADER_PROGRAM_H

#include "Shader_program.h"

namespace vup
{
    class V_G_F_shader_program : public vup::Shader_program {
    public:
        V_G_F_shader_program(const std::shared_ptr<vup::Vertex_shader>& vertex,
                             const std::shared_ptr<vup::Geometry_shader>& geometry,
                             const std::shared_ptr<vup::Fragment_shader>& fragment,
                             vup::gl::Introspection introspection_flag = vup::gl::Introspection::basic);
        void reload() override;
    protected:
        void attach_shaders() const override;
        void detach_shaders() const override;
    private:
        std::shared_ptr<vup::Vertex_shader> m_vertex;
        std::shared_ptr<vup::Geometry_shader> m_geometry;
        std::shared_ptr<vup::Fragment_shader> m_fragment;
    };
}


#endif //VUP_V_G_F_SHADER_PROGRAM_H
