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
    struct Grid_consts {
        glm::vec3 cell_size;
        int cell_capacity;
        glm::vec3 grid_size;
        int cell_count;
        glm::vec4 grid_pos;
    };    
}

#endif // VUP_ACCELERATION_STRUCTURE_H