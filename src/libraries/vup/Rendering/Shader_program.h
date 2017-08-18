//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_SHADER_PROGRAM_H
#define VUP_SHADER_PROGRAM_H

#include <memory>
#include "vup/Rendering/Shader.h"

namespace vup
{
    class Shader_program {
    public:
        Shader_program();
        void use();
    protected:
        void link_program();
        virtual void attach_shaders()= 0;
        GLuint m_program_id;
    };
}


#endif //VUP_SHADER_PROGRAM_H
