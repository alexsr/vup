//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Compute_shader.h"

vup::Compute_shader::Compute_shader(const filesystem::path& path)
        : vup::Shader(path, GL_COMPUTE_SHADER) {
}
