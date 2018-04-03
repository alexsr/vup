//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Geometric_primitives.h"

vup::Rectangle::Rectangle(float x, float y) {
    vertices = {
        {-x / 2.0f, -y / 2.0f, 0.0f, 1.0f},
        {x / 2.0f, -y / 2.0f, 0.0f, 1.0f},
        {-x / 2.0f, y / 2.0f, 0.0f, 1.0f},
        {x / 2.0f, -y / 2.0f, 0.0f, 1.0f},
        {-x / 2.0f, y / 2.0f, 0.0f, 1.0f},
        {x / 2.0f, y / 2.0f, 0.0f, 1.0f}
    };
    normals = {
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}
    };
    uv_coords = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f}
    };
    indices = {0, 1, 2, 3, 4, 5};
}

vup::Rectangle_strip::Rectangle_strip(float x, float y) {
    vertices = {
        {-x / 2.0f, -y / 2.0f, 0.0f, 1.0f},
        {x / 2.0f, -y / 2.0f, 0.0f, 1.0f},
        {-x / 2.0f, y / 2.0f, 0.0f, 1.0f},
        {x / 2.0f, y / 2.0f, 0.0f, 1.0f}
    };
    normals = {
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}
    };
    uv_coords = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 0.0f}
    };
    indices = {0, 1, 2, 1, 2, 3};
}

vup::Cube::Cube(float r) : Cube(r, r, r) {
}

vup::Cube::Cube(float x, float y, float z) {
    float x_lower = -x / 2.0f;
    float x_upper = x / 2.0f;
    float y_lower = -y / 2.0f;
    float y_upper = y / 2.0f;
    float z_lower = -z / 2.0f;
    float z_upper = z / 2.0f;
    vertices = {
        {x_lower, y_lower, z_upper, 1.0f},
        {x_upper, y_lower, z_upper, 1.0f},
        {x_upper, y_upper, z_upper, 1.0f},
        {x_upper, y_upper, z_upper, 1.0f},
        {x_lower, y_upper, z_upper, 1.0f},
        {x_lower, y_lower, z_upper, 1.0f},

        {x_lower, y_lower, z_lower, 1.0f},
        {x_lower, y_lower, z_upper, 1.0f},
        {x_lower, y_upper, z_upper, 1.0f},
        {x_lower, y_upper, z_upper, 1.0f},
        {x_lower, y_upper, z_lower, 1.0f},
        {x_lower, y_lower, z_lower, 1.0f},

        {x_upper, y_lower, z_upper, 1.0f},
        {x_upper, y_lower, z_lower, 1.0f},
        {x_upper, y_upper, z_lower, 1.0f},
        {x_upper, y_upper, z_lower, 1.0f},
        {x_upper, y_upper, z_upper, 1.0f},
        {x_upper, y_lower, z_upper, 1.0f},

        {x_lower, y_upper, z_upper, 1.0f},
        {x_upper, y_upper, z_upper, 1.0f},
        {x_upper, y_upper, z_lower, 1.0f},
        {x_upper, y_upper, z_lower, 1.0f},
        {x_lower, y_upper, z_lower, 1.0f},
        {x_lower, y_upper, z_upper, 1.0f},

        {x_lower, y_lower, z_lower, 1.0f},
        {x_upper, y_lower, z_lower, 1.0f},
        {x_upper, y_lower, z_upper, 1.0f},
        {x_upper, y_lower, z_upper, 1.0f},
        {x_lower, y_lower, z_upper, 1.0f},
        {x_lower, y_lower, z_lower, 1.0f},

        {x_lower, y_upper, z_lower, 1.0f},
        {x_upper, y_upper, z_lower, 1.0f},
        {x_upper, y_lower, z_lower, 1.0f},
        {x_upper, y_lower, z_lower, 1.0f},
        {x_lower, y_lower, z_lower, 1.0f},
        {x_lower, y_upper, z_lower, 1.0f},
    };
    normals = {
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},

        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},

        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},

        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},

        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},

        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f}
    };
    uv_coords = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f},

        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
        {0.0f, 0.0f}
    };
    indices = {
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35
    };
}

vup::Sphere::Sphere(float r, int hres, int vres) {
    vertices.resize(hres * vres * 6);
    normals.resize(hres * vres * 6);
    uv_coords.resize(hres * vres * 6);
    indices.resize(hres * vres * 6);
    float d_h = 2 * glm::pi<float>() / ((float) hres);
    float d_v = glm::pi<float>() / ((float) vres);
    int n = 0;
    // Vertices are created inside this loop.
    for (int i = 0; i < hres; i++) {
        float h = i * d_h;
        float hn = h + d_h;
        for (int j = 0; j < vres; j++) {
            float v = j * d_v;
            float vn = v + d_v;

            // The sphere is consists of multiple triangles where 2 triangles make a plane.
            // These 4 points are the corners of said plane. To create a triangle 3 of these corners are
            // used counterclockwise with the 2nd triangle's first point being the 1st last point.
            // Normal vectors are the same as the points without the radius multiplied.
            glm::vec4 p0(glm::cos(h) * glm::sin(v), glm::sin(h) * glm::sin(v),
                         glm::cos(v), 1.0f);
            glm::vec4 p1(glm::cos(h) * glm::sin(vn), glm::sin(h) * glm::sin(vn),
                         glm::cos(vn), 1.0f);
            glm::vec4 p2(glm::cos(hn) * glm::sin(v), glm::sin(hn) * glm::sin(v),
                         glm::cos(v), 1.0f);
            glm::vec4 p3(glm::cos(hn) * glm::sin(vn), glm::sin(hn) * glm::sin(vn),
                         glm::cos(vn), 1.0f);
            vertices.at(n) = p0 * r;
            normals.at(n++) = glm::vec3(p0);
            vertices.at(n) = p1 * r;
            normals.at(n++) = glm::vec3(p1);
            vertices.at(n) = p3 * r;
            normals.at(n++) = glm::vec3(p3);
            vertices.at(n) = p3 * r;
            normals.at(n++) = glm::vec3(p3);
            vertices.at(n) = p2 * r;
            normals.at(n++) = glm::vec3(p2);
            vertices.at(n) = p0 * r;
            normals.at(n++) = glm::vec3(p0);
        }
    }
}
