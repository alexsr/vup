//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_GEOMETRIC_PRIMITIVES_H
#define VUP_GEOMETRIC_PRIMITIVES_H

#include <vup/Core/vup.h>
#include <vector>

namespace vup
{
    struct Geometric_primitive {
        std::vector<glm::vec4> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv_coords;
        std::vector<unsigned int> indices;
    };

    struct Rectangle : Geometric_primitive {
        explicit Rectangle(float x = 1.0f, float y = 1.0f);
    };

    struct Rectangle_strip : Geometric_primitive {
        explicit Rectangle_strip(float x = 1.0f, float y = 1.0f);
    };

    struct Cube : Geometric_primitive {
        explicit Cube(float x = 1.0f, float y = 1.0f, float z = 1.0f);
    };

    struct Sphere : Geometric_primitive {
        explicit Sphere(float r = 0.1f, int hres = 10, int vres = 10);
    };
}

#endif //VUP_GEOMETRIC_PRIMITIVES_H
