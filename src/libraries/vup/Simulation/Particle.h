//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_PARTICLE_H
#define VUP_PARTICLE_H

#include <vup/Core/vup.h>

namespace vup
{
    struct SPH_particle {
        glm::vec4 pos;
        glm::vec4 vel;
        glm::vec4 acc;
        glm::vec4 force;
        float mass;
        float rest_density;
        float pressure;
        float density;
    };

    struct Collision_particle {
        glm::vec4 pos;
        glm::vec4 old_pos;
        glm::vec4 vel;
        glm::vec4 momentum;
        float mass;
        float rest_density;
        float pressure;
        float density;
    };
}

#endif //VUP_PARTICLE_H
