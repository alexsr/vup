//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_BOUNDING_BOX_UTILS_H
#define VUP_BOUNDING_BOX_UTILS_H

#include <vup/Core/vup.h>
#include <vector>

namespace vup
{
    namespace vox
    {
        struct Bounds {
            glm::vec4 min = glm::vec4(FLT_MAX, FLT_MAX, FLT_MAX, 1.0f);
            glm::vec4 max = glm::vec4(-FLT_MAX, -FLT_MAX, -FLT_MAX, 1.0f);
        };
        Bounds reduce_bounds(const std::vector<Bounds>& bounds);
    }
}

#endif //VUP_BOUNDING_BOX_UTILS_H
