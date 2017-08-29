//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Geometry_primitives.h"

vup::Quad::Quad(float x, float y) {
    vertices = {-x/2.0f, -y/2.0f, 0.0f, 1.0f,
            x/2.0f, -y/2.0f, 0.0f, 1.0f,
            -x/2.0f, y/2.0f, 0.0f, 1.0f,
            x/2.0f, y/2.0f, 0.0f, 1.0f
    };
}