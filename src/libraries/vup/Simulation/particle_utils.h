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
    struct SPH_demo_constants {
        explicit SPH_demo_constants(float r = 0.1f)
                : SPH_demo_constants(r, 4*r) {}
        explicit SPH_demo_constants(float r, float h) {
            radius = r;
            smoothing_length = h;
            float h3 = h * h * h;
            float h6 = h3 * h3;
            poly_six_const = 315.0f/(64.0f * glm::pi<float>() * h3 * h6);
            spiky_const = 45.0f/(glm::pi<float>() * h6);
        }
        float radius;
        float smoothing_length;
        float poly_six_const;
        float spiky_const;
    };

    std::vector<SPH_particle> create_uniform_SPH_particles(float radius,
                                                           float lower,
                                                           float upper,
                                                           float rest_density,
                                                           float h) {
        float step = radius * 2.0f;
        float mass = rest_density * h * h * h;
        auto n = static_cast<int>((upper - lower) / step) - 1;
        std::vector<SPH_particle> result(static_cast<unsigned long>(n * n * n));
        for (unsigned long i = 0; i < n; i++) {
            for (unsigned long j = 0; j < n; j++) {
                for (unsigned long k = 0; k < n; k++) {
                    result.at(i*n*n + j*n + k).pos = glm::vec4(i * step + lower + radius,
                                                               j * step + lower + radius,
                                                               k * step + lower + radius, 1.0f);
                    result.at(i*n*n + j*n + k).acc = glm::vec4(0.0f);
                    result.at(i*n*n + j*n + k).vel = glm::vec4(0.0f);
                    result.at(i*n*n + j*n + k).force = glm::vec4(0.0f);
                    result.at(i*n*n + j*n + k).mass = mass;
                    result.at(i*n*n + j*n + k).rest_density = rest_density;
                    result.at(i*n*n + j*n + k).density = rest_density;
                }
            }
        }
        return result;
    }
}

#endif //VUP_PARTICLE_UTILS_H
