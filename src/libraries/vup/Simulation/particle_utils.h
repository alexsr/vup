//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_PARTICLE_UTILS_H
#define VUP_PARTICLE_UTILS_H

#include "Particle.h"

namespace vup
{
    struct Boundary_particle {
        glm::vec4 pos;
        glm::vec4 vel;
    };

    inline std::vector<Boundary_particle> create_boundary_box(glm::vec3 size, glm::vec4 box_mid,
                                                              float radius) {
        size += glm::vec3(2 * radius);
        glm::vec4 box_min = box_mid - glm::vec4(size / 2.0f, 0.0f);
        glm::vec3 sampling = glm::ceil(size / (2.0f * radius));
        glm::vec3 sampling_dist = size / sampling;
        const auto res_size = static_cast<int>(2 * (sampling.x - 1) * (sampling.y - 1) + (sampling.z + 1) * 2 * (
                                                  sampling.x + 2 + sampling.y));
        std::vector<Boundary_particle> res(res_size);
        unsigned long long id = 0;
        for (unsigned long long i = 1; i < sampling.x; i++) {
            for (unsigned long long j = 1; j < sampling.y; j++) {
                res.at(id++).pos = box_min + glm::vec4(i * sampling_dist.x, j * sampling_dist.y, 0, 0);
                res.at(id++).pos = box_min + glm::vec4(i * sampling_dist.x, j * sampling_dist.y,
                                                       sampling.z * sampling_dist.z, 0);
            }
        }

        for (unsigned long long k = 0; k <= sampling.z; k++) {
            for (unsigned long long i = 0; i <= sampling.x; i++) {
                res.at(id++).pos = box_min + glm::vec4(i * sampling_dist.x, 0, k * sampling_dist.z, 0);
                res.at(id++).pos = box_min + glm::vec4(i * sampling_dist.x, sampling.y * sampling_dist.y,
                                                       k * sampling_dist.z, 0);
            }
            for (unsigned long long j = 0; j <= sampling.y; j++) {
                res.at(id++).pos = box_min + glm::vec4(0, j * sampling_dist.y, k * sampling_dist.z, 0);
                res.at(id++).pos = box_min + glm::vec4(sampling.x * sampling_dist.x, j * sampling_dist.y,
                                                       k * sampling_dist.z, 0);
            }
        }
        return res;
    }

    struct SPH_demo_constants {
        SPH_demo_constants(const float smoothing_length, const float mass_scaling, const float dt)
            : dt(dt), h(smoothing_length), kernel_const(0), kernel_grad_const(0), kernel_laplace_const(0),
              visc_const(0) {
            inv_dt = 1.0f / dt;
            dt2 = dt * dt;
            h2 = h * h;
            eps = 0.0000001f;
            r = 1.0f / glm::pow(4.0f / 3.0f * glm::pi<float>(), 1.0f / 3.0f) * h * mass_scaling;
        }

        float dt;
        float inv_dt;
        float dt2;
        float eps;
        float r;
        float h;
        float h2;
        float kernel_const;
        float kernel_grad_const;
        float kernel_laplace_const;
        float visc_const;
    };

    struct Mueller_SPH_demo_constants : SPH_demo_constants {
        Mueller_SPH_demo_constants(const float smoothing_length, const float mass_scaling, const float dt)
            : SPH_demo_constants(smoothing_length, mass_scaling, dt) {
            const auto h3 = h2 * h;
            const auto h6 = h3 * h3;
            kernel_const = 315.0f / (64.0f * glm::pi<float>() * h3 * h6);
            kernel_grad_const = 45.0f / (glm::pi<float>() * h6);
            kernel_laplace_const = 945.0f / (32.0f * glm::pi<float>() * h3 * h6);
            visc_const = kernel_grad_const;
        }

    };

    struct IISPH_demo_constants : SPH_demo_constants {
        IISPH_demo_constants(const float smoothing_length, const float mass_scaling, const float dt)
            : SPH_demo_constants(smoothing_length, mass_scaling, dt) {
            const auto h3 = h2 * h;
            const auto h6 = h3 * h3;
            kernel_const = 8.0f / (glm::pi<float>() * h3);
            kernel_grad_const = 48.0f / (glm::pi<float>() * h3);
            kernel_laplace_const = 945.0f / (32.0f * glm::pi<float>() * h3 * h6);
            visc_const = 45.0f / (glm::pi<float>() * h6);
        }
    };

    struct DFSPH_heat_demo_constants : SPH_demo_constants {
        DFSPH_heat_demo_constants(const float smoothing_length, const float mass_scaling, const float dt)
            : SPH_demo_constants(smoothing_length, mass_scaling, dt) {
            const auto h3 = h2 * h;
            const auto h6 = h3 * h3;
            kernel_const = 8.0f / (glm::pi<float>() * h3);
            kernel_grad_const = 48.0f / (glm::pi<float>() * h3);
            kernel_laplace_const = 945.0f / (32.0f * glm::pi<float>() * h3 * h6);
            visc_const = 45.0f / (glm::pi<float>() * h6);
        }
    };

    struct IISPH_heat_demo_constants : IISPH_demo_constants {
        IISPH_heat_demo_constants(const float smoothing_length, const float mass_scaling, const float dt,
                                  const float temperature_in_celcius)
            : IISPH_demo_constants(smoothing_length, mass_scaling, dt) {
            temperature = temperature_in_celcius + 273.15f;
            heat_const = 0.024f / 1.0035f;
        }

        float temperature{};
        float heat_const{};
    };
}

#endif //VUP_PARTICLE_UTILS_H
