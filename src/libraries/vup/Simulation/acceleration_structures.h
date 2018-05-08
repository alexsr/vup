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
    struct Compact_grid_params {
        Compact_grid_params(float grid_size_x, float grid_size_y, float grid_size_z, float cell_size_x,
                            float cell_size_y, float cell_size_z, glm::vec4 pos = glm::vec4(0.0f))
            : grid_pos(pos) {
            const float cell_count_x = glm::ceil(grid_size_x / cell_size_x);
            const float cell_count_y = glm::ceil(grid_size_y / cell_size_y);
            const float cell_count_z = glm::ceil(grid_size_z / cell_size_z);
            cell_count = glm::ivec4(cell_count_x, cell_count_y, cell_count_z, 0);
            total_cell_count = static_cast<int>(cell_count_x * cell_count_y * cell_count_z);
            cell_size = glm::vec4(cell_size_x, cell_size_y, cell_size_z, 0);
            const auto half_size = glm::vec4(cell_size_x * cell_count_x, cell_size_y * cell_count_y,
                                             cell_size_z * cell_count_z, 0) / 2.0f;
            grid_min = grid_pos - half_size;
        }

        glm::vec4 grid_min;
        glm::ivec4 cell_count;
        glm::vec4 cell_size;
        glm::vec4 grid_pos;
        int total_cell_count;
    };

    struct Cube_compact_grid_params : Compact_grid_params {
        Cube_compact_grid_params(float grid_size, float cell_size,
                                 glm::vec4 pos = glm::vec4(0.0f))
            : Compact_grid_params(grid_size, grid_size, grid_size, cell_size,
                                  cell_size, cell_size, pos) {
        }
    };

    struct Uniform_grid_params : Compact_grid_params {
        Uniform_grid_params(float grid_size_x, float grid_size_y, float grid_size_z,
                            float cell_size_x, float cell_size_y, float cell_size_z,
                            int cell_capacity, glm::vec4 pos = glm::vec4(0.0f))
            : Compact_grid_params(grid_size_x, grid_size_y, grid_size_z, cell_size_x, cell_size_y, cell_size_z, pos),
              cell_capacity(cell_capacity) {
            grid_capacity = total_cell_count * cell_capacity;
        }

        int cell_capacity;
        int grid_capacity;
    };

    struct Cube_uniform_grid_params : Uniform_grid_params {
        Cube_uniform_grid_params(float grid_size, float cell_size, int cell_capacity,
                                 glm::vec4 pos = glm::vec4(0.0f))
            : Uniform_grid_params(grid_size, grid_size, grid_size, cell_size,
                                  cell_size, cell_size, cell_capacity, pos) {
        }
    };
}

#endif // VUP_ACCELERATION_STRUCTURE_H
