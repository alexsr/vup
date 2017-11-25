//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Geometric_primitives.h"

vup::Rectangle::Rectangle(float x, float y) {
    vertices = {{-x/2.0f, -y/2.0f, 0.0f, 1.0f},
                {x/2.0f, -y/2.0f, 0.0f, 1.0f},
                {-x/2.0f, y/2.0f, 0.0f, 1.0f},
                {x/2.0f, -y/2.0f, 0.0f, 1.0f},
                {-x/2.0f, y/2.0f, 0.0f, 1.0f},
                {x/2.0f, y/2.0f, 0.0f, 1.0f}
    };
    normals = {{0.0f, 0.0f, 1.0f},
              {0.0f, 0.0f, 1.0f},
              {0.0f, 0.0f, 1.0f},
              {0.0f, 0.0f, 1.0f},
              {0.0f, 0.0f, 1.0f},
              {0.0f, 0.0f, 1.0f}
    };
    uv_coords = {{0.0f, 0.0f},
          {1.0f, 0.0f},
          {0.0f, 1.0f},
          {1.0f, 0.0f},
          {0.0f, 1.0f},
          {1.0f, 1.0f}
    };
    indices = {0, 1, 2, 3, 4, 5};
}

vup::Rectangle_strip::Rectangle_strip(float x, float y) {
    vertices = {{-x/2.0f, -y/2.0f, 0.0f, 1.0f},
                 {x/2.0f, -y/2.0f, 0.0f, 1.0f},
                 {-x/2.0f, y/2.0f, 0.0f, 1.0f},
				 {x/2.0f, y/2.0f, 0.0f, 1.0f}
    };
    normals = {{0.0f, 0.0f, 1.0f},
               {0.0f, 0.0f, 1.0f},
               {0.0f, 0.0f, 1.0f},
               {0.0f, 0.0f, 1.0f}
    };
    uv_coords = {{ 0.0f, 0.0f},
                 {1.0f, 0.0f},
                 {0.0f, 1.0f},
                 {1.0f, 0.0f}
    };
    indices = {0, 1, 2, 1, 2, 3};
}
