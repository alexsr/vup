//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_GEOMETRY_PRIMITIVES_H
#define VUP_GEOMETRY_PRIMITIVES_H

#include <vector>

namespace vup
{
    struct Quad {
        explicit Quad(float x = 1.0f, float y = 1.0f);
        std::vector<float> vertices;
    };
}

#endif //VUP_GEOMETRY_PRIMITIVES_H
