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
        SPH_demo_constants(const float smoothing_length, const float mass_scaling)
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
        Mueller_SPH_demo_constants(const float smoothing_length, const float mass_scaling)
            : SPH_demo_constants(smoothing_length, mass_scaling) {
            const auto h3 = h * h * h;
            const auto h6 = h3 * h3;
            kernel_const = 315.0f / (64.0f * glm::pi<float>() * h3 * h6);
            kernel_grad_const = 45.0f / (glm::pi<float>() * h6);
            kernel_laplace_const = 945.0f / (32.0f * glm::pi<float>() * h3 * h6);
            visc_const = kernel_grad_const;
        }

    };

    struct IISPH_demo_constants : SPH_demo_constants {
        IISPH_demo_constants(const float smoothing_length, const float mass_scaling)
            : SPH_demo_constants(smoothing_length, mass_scaling) {
            const auto h3 = h * h * h;
            const auto h6 = h3 * h3;
            kernel_const = 8.0f / (glm::pi<float>() * h3);
            kernel_grad_const = 48.0f / (glm::pi<float>() * h3);
            kernel_laplace_const = -945.0f / (32.0f * glm::pi<float>() * h3 * h6);
            visc_const = 45.0f / (glm::pi<float>() * h6);
        }
    };

    struct IISPH_heat_demo_constants : IISPH_demo_constants {
        IISPH_heat_demo_constants(const float smoothing_length, const float mass_scaling, const float temperature_in_celcius)
            : IISPH_demo_constants(smoothing_length, mass_scaling), temperature(temperature_in_celcius + 273.15f) {
            heat_const = 0.024f / 1.0035f;
        }
        float temperature;
        float heat_const;
    };
}

#endif //VUP_PARTICLE_UTILS_H
