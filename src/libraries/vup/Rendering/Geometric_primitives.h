//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_GEOMETRIC_PRIMITIVES_H
#define VUP_GEOMETRIC_PRIMITIVES_H

#include <vector>
#include "vup/Core/vup.h"

namespace vup
{
    struct Geometric_primitive {
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec3> normal;
        std::vector<glm::vec2> uv;
        std::vector<unsigned int> indices;
    };

    struct Quad : public Geometric_primitive {
        explicit Quad(float x = 1.0f, float y = 1.0f);
    };
}

#endif //VUP_GEOMETRIC_PRIMITIVES_H
