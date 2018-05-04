//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Particle.h"

std::vector<vup::SPH_particle> vup::create_uniform_SPH_particles(const float radius,
                                                                 const float lower,
                                                                 const float upper,
                                                                 const float rest_density,
                                                                 const float h) {
    const auto step = radius * 2.0f;
    const auto mass = rest_density * h * h * h;
    const auto n = static_cast<unsigned int>(glm::max((upper - lower) / step - 1, 0.0f));
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

std::vector<vup::IISPH_particle> vup::create_uniform_IISPH_particles(const float r, const float h,
                                                                     const float lower, const float upper,
                                                                     const float rest_density) {
    const auto mass = rest_density * h * h * h;
    const auto step = r * 2.0f;
    const auto n = static_cast<unsigned long>((upper - lower) / step) - 1;
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

std::vector<vup::IISPH_heat_particle> vup::create_uniform_IISPH_heat_particles(const float r, const float h,
                                                                               const float lower, const float upper,
                                                                               const float rest_density,
                                                                               const float visc,
                                                                               const float temperature_in_celsius,
                                                                               const float max_latent_heat,
                                                                               const float temp_melt) {
    const auto mass = rest_density * h * h * h;
    const auto step = r * 2.0f;
    const auto n = static_cast<unsigned long>((upper - lower) / step) - 1;
    std::vector<IISPH_heat_particle> result(static_cast<unsigned long>(n * n * n));
    for (unsigned long i = 0; i < n; i++) {
        for (unsigned long j = 0; j < n; j++) {
            for (unsigned long k = 0; k < n; k++) {
                result.at(i * n * n + j * n + k).pos = glm::vec4(i * step + lower + r,
                                                                 j * step + lower + r,
                                                                 k * step + lower + r, 1.0f);
                result.at(i * n * n + j * n + k).mass = mass;
                result.at(i * n * n + j * n + k).rest_density = rest_density;
                result.at(i * n * n + j * n + k).temperature = temperature_in_celsius + 273.15f;
                result.at(i * n * n + j * n + k).heat_const = 0.591f / 4181.3f; // 0.00143f;
                result.at(i * n * n + j * n + k).latent_heat_max = max_latent_heat;
                result.at(i * n * n + j * n + k).temp_melt = temp_melt;
                result.at(i * n * n + j * n + k).viscosity = visc;

            }
        }
    }
    return result;
}

std::vector<vup::DFSPH_heat_particle> vup::create_DFSPH_heat_particles(const float r,
                                                                       const float h, const float lower,
                                                                       const float upper,
                                                                       const float rest_density,
                                                                       const float viscosity,
                                                                       const float temperature,
                                                                       const float latent_heat_max) {
    const auto mass = rest_density * h * h * h;
    const auto step = r * 2.0f;
    const auto n = static_cast<unsigned long>((upper - lower) / step);
    std::vector<DFSPH_heat_particle> result(static_cast<unsigned long>(n * n * n));
    for (unsigned long i = 0; i < n; i++) {
        for (unsigned long j = 0; j < n; j++) {
            for (unsigned long k = 0; k < n; k++) {
                result.at(i * n * n + j * n + k).pos = glm::vec4(i * step + lower + r,
                                                                 j * step + lower + r,
                                                                 k * step + lower + r, 1.0f);
                result.at(i * n * n + j * n + k).mass = mass;
                result.at(i * n * n + j * n + k).rest_density = rest_density;
                result.at(i * n * n + j * n + k).density = rest_density;
                result.at(i * n * n + j * n + k).vel_diff = glm::vec4(0);
                result.at(i * n * n + j * n + k).viscosity = viscosity;
                result.at(i * n * n + j * n + k).temperature = temperature + 273.15f;
                result.at(i * n * n + j * n + k).heat_const = 0.591f / 4181.3f; // 0.00143f;
                result.at(i * n * n + j * n + k).heat_buffer = latent_heat_max;
            }
        }
    }
    return result;
}
