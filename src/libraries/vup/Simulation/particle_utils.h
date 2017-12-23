//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_PARTICLE_UTILS_H
#define VUP_PARTICLE_UTILS_H

#include "Particle.h"
#include <vector>

namespace vup
{
    std::vector<SPH_particle> fill_uniformly(float radius, float lower, float upper,
                                             float rest_density) {
        float step = radius * 2.0f + 0.0001f;
        float mass = 4.0f/3.0f * glm::pi<float>() * rest_density * radius * radius * radius;
        auto n = static_cast<int>((upper - lower) / step) - 1;
        std::vector<SPH_particle> result(static_cast<unsigned long>(n * n * n));
        for (unsigned long i = 0; i < n; i++) {
            for (unsigned long j = 0; j < n; j++) {
                for (unsigned long k = 0; k < n; k++) {
                    result.at(i*n*n + j*n + k).pos = glm::vec4(i * step + lower + radius,
                                                               j * step + lower + radius,
                                                               k * step + lower + radius, 1.0f);
                    result.at(i*n*n + j*n + k).old_pos = glm::vec4(i * step + lower + radius,
                                                                   j * step + lower + radius,
                                                                   k * step + lower + radius, 1.0f);
                    result.at(i*n*n + j*n + k).vel = glm::vec4(0.0f);
                    result.at(i*n*n + j*n + k).force = glm::vec4(0.0f);
                    result.at(i*n*n + j*n + k).mass = mass;
                    result.at(i*n*n + j*n + k).rest_density = rest_density;
                    result.at(i*n*n + j*n + k).density = rest_density;
                    if (i * n * n + j * n + k < n * n * n / 2) {
                        result.at(i*n*n + j*n + k).rest_density *= 2;
                        result.at(i*n*n + j*n + k).density *= 2;
                    }
                }
            }
        }
        return result;
    }
}

#endif //VUP_PARTICLE_UTILS_H
