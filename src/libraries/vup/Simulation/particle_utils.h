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
        SPH_demo_constants(float smoothing_length, float mass_scaling)
            : kernel_const(0), kernel_grad_const(0), kernel_laplace_const(0), visc_const(0) {
            h = smoothing_length;
            r = 1.0f / glm::pow(4.0f / 3.0f * glm::pi<float>(), 1.0f / 3.0f) * h * mass_scaling;
        }

        float r;
        float h;
        float kernel_const;
        float kernel_grad_const;
        float kernel_laplace_const;
        float visc_const;
    };

    struct Mueller_SPH_demo_constants : SPH_demo_constants {
        Mueller_SPH_demo_constants(float smoothing_length, float mass_scaling) : SPH_demo_constants(
            smoothing_length, mass_scaling) {
            float h3 = h * h * h;
            float h6 = h3 * h3;
            kernel_const = 315.0f / (64.0f * glm::pi<float>() * h3 * h6);
            kernel_grad_const = 45.0f / (glm::pi<float>() * h6);
            kernel_laplace_const = 945.0f / (32.0f * glm::pi<float>() * h3 * h6);
            visc_const = kernel_grad_const;
        }

    };

    struct IISPH_demo_constants : SPH_demo_constants {
        IISPH_demo_constants(float smoothing_length, float mass_scaling) : SPH_demo_constants(
            smoothing_length, mass_scaling) {
            float h3 = h * h * h;
            float h6 = h3 * h3;
            kernel_const = 8.0f / (glm::pi<float>() * h3);
            kernel_grad_const = 48.0f / (glm::pi<float>() * h3);
            visc_const = 45.0f / (glm::pi<float>() * h6);
        }
    };

    inline std::vector<SPH_particle> create_uniform_SPH_particles(float radius,
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
                    result.at(i * n * n + j * n + k).pos = glm::vec4(i * step + lower + radius,
                                                                     j * step + lower + radius,
                                                                     k * step + lower + radius, 1.0f);
                    result.at(i * n * n + j * n + k).acc = glm::vec4(0.0f);
                    result.at(i * n * n + j * n + k).vel = glm::vec4(0.0f);
                    result.at(i * n * n + j * n + k).force = glm::vec4(0.0f);
                    result.at(i * n * n + j * n + k).mass = mass;
                    result.at(i * n * n + j * n + k).rest_density = rest_density;
                    result.at(i * n * n + j * n + k).density = rest_density;
                }
            }
        }
        return result;
    }

    inline std::vector<IISPH_particle> create_uniform_IISPH_particles(float r, float h, float lower, float upper,
                                                                      float rest_density) {
        float mass = rest_density * h * h * h;
        float step = r * 2.0f;
        auto n = static_cast<unsigned long>((upper - lower) / step) - 1;
        std::vector<IISPH_particle> result(static_cast<unsigned long>(n * n * n));
        for (unsigned long i = 0; i < n; i++) {
            for (unsigned long j = 0; j < n; j++) {
                for (unsigned long k = 0; k < n; k++) {
                    result.at(i * n * n + j * n + k).pos = glm::vec4(i * step + lower + r,
                                                                     j * step + lower + r,
                                                                     k * step + lower + r, 1.0f);
                    result.at(i * n * n + j * n + k).vel = glm::vec4(0.0f);
                    result.at(i * n * n + j * n + k).vel_adv = glm::vec4(0.0f);
                    result.at(i * n * n + j * n + k).pressure_force = glm::vec4(0.0f);
                    result.at(i * n * n + j * n + k).mass = mass;
                    result.at(i * n * n + j * n + k).rest_density = rest_density;
                    result.at(i * n * n + j * n + k).density = 0.0f;
                    result.at(i * n * n + j * n + k).density_adv = 0.0f;
                    result.at(i * n * n + j * n + k).pressure = 0.0f;
                    result.at(i * n * n + j * n + k).last_pressure = 0.0f;
                    result.at(i * n * n + j * n + k).aii = 0.0f;
                    result.at(i * n * n + j * n + k).aij_pj_sum = 0.0f;
                    result.at(i * n * n + j * n + k).dii = glm::vec4(0.0f);
                    result.at(i * n * n + j * n + k).dij_pj_sum = glm::vec4(0.0f);
                }
            }
        }
        return result;
    }
}

#endif //VUP_PARTICLE_UTILS_H
