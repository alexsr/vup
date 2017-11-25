//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_COMPUTE_SHADER_H
#define VUP_COMPUTE_SHADER_H

#include <vup/Rendering/Shader.h>

namespace vup
{
    class Compute_shader : public Shader {
    public:
        explicit Compute_shader(const filesystem::path& path);
    };
}

#endif //VUP_COMPUTE_SHADER_H
