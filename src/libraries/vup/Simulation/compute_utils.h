//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_COMPUTE_UTILS_H
#define VUP_COMPUTE_UTILS_H

#include <vup/Utility/File_loader.h>
#include <utility>
#include "vup/Shader/Compute_shader.h"

namespace vup
{
    class Reduction {
    protected:
        Reduction(const filesystem::path& shader, const std::shared_ptr<Storage_buffer>& buffer);
        std::vector<Compute_shader> m_shaders;
        std::shared_ptr<Storage_buffer> m_buffer;
    };

    inline Reduction::Reduction(const filesystem::path& shader, const std::shared_ptr<Storage_buffer>& buffer)
        : m_buffer(buffer) {
    }
}

#endif // VUP_COMPUTE_UTILS_H
