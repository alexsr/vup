//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_ACCELERATION_STRUCTURE_H
#define VUP_ACCELERATION_STRUCTURE_H

#include <vup/Core/vup.h>

namespace vup
{
    struct Uniform_grid_params {
        Uniform_grid_params(float grid_size_x, float grid_size_y, float grid_size_z, float cell_size_x,
                            float cell_size_y, float cell_size_z, int cell_capacity,
                            glm::vec4 pos = glm::vec4(0.0f));

        glm::vec4 grid_min;
        glm::ivec4 cell_count;
        glm::vec4 cell_size;
        glm::vec4 grid_pos;
        int cell_capacity;
        int total_cell_count;
        int grid_capacity;
    };

    inline Uniform_grid_params::Uniform_grid_params(float grid_size_x, float grid_size_y, float grid_size_z,
                                                    float cell_size_x, float cell_size_y, float cell_size_z,
                                                    int cell_capacity, glm::vec4 pos)
        : grid_pos(pos), cell_capacity(cell_capacity) {
        const float cell_count_x = glm::ceil(grid_size_x / cell_size_x);
        const float cell_count_y = glm::ceil(grid_size_y / cell_size_y);
        const float cell_count_z = glm::ceil(grid_size_z / cell_size_z);
        cell_count = glm::ivec4(cell_count_x, cell_count_y, cell_count_z, 0);
        total_cell_count = cell_count_x * cell_count_y * cell_count_z;
        grid_capacity = total_cell_count * cell_capacity;
        cell_size = glm::vec4(cell_size_x, cell_size_y, cell_size_z, 0);
        const auto half_size = glm::vec4(cell_size_x * cell_count_x, cell_size_y * cell_count_y,
                                         cell_size_z * cell_count_z, 0) / 2.0f;
        grid_min = grid_pos - half_size;
    }

    struct Cube_uniform_grid_params : Uniform_grid_params {
        Cube_uniform_grid_params(float grid_size, float cell_size, int cell_capacity,
                                 glm::vec4 pos = glm::vec4(0.0f))
            : Uniform_grid_params(grid_size, grid_size, grid_size, cell_size,
                                  cell_size, cell_size, cell_capacity, pos) {
        }
    };
}

#endif // VUP_ACCELERATION_STRUCTURE_H
