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
    const auto n = static_cast<int>((upper - lower) / step) - 1;
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
                                                                               const float temperature_in_celsius) {
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
                result.at(i * n * n + j * n + k).temperature_grad = glm::vec4(0.0f);
                result.at(i * n * n + j * n + k).temperature = temperature_in_celsius + 273.15f;
                result.at(i * n * n + j * n + k).temp_change = 0.0f;
                result.at(i * n * n + j * n + k).heat_const = 0.591f / 4181.3f; // 0.00143f;
                result.at(i * n * n + j * n + k).heat_buffer = 0.0f;
            }
        }
    }
    return result;
}

std::vector<vup::Phase_change_particle> vup::create_uniform_Phase_change_particles(float r, float h, float lower,
                                                                                   float upper, float rest_density,
                                                                                   float temperature,
                                                                                   float rest_temperature,
                                                                                   float rest_viscosity, float visc_alpha) {
    const auto mass = rest_density * h * h * h;
    const auto step = r * 2.0f;
    const auto n = static_cast<unsigned long>((upper - lower) / step) - 1;
    std::vector<Phase_change_particle> result(static_cast<unsigned long>(n * n * n));
    for (unsigned long i = 0; i < n; i++) {
        for (unsigned long j = 0; j < n; j++) {
            for (unsigned long k = 0; k < n; k++) {
                result.at(i * n * n + j * n + k).pos = glm::vec4(i * step + lower + r,
                                                                 j * step + lower + r,
                                                                 k * step + lower + r, 1.0f);
                result.at(i * n * n + j * n + k).vel = glm::vec4(0.0f);
                result.at(i * n * n + j * n + k).vel_adv = glm::vec4(0.0f);
                result.at(i * n * n + j * n + k).pressure_accel = glm::vec4(0.0f);
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
                result.at(i * n * n + j * n + k).temperature_grad = glm::vec4(0.0f);
                result.at(i * n * n + j * n + k).temperature = temperature + 273.15f;
                result.at(i * n * n + j * n + k).rest_temperature = rest_temperature + 273.15f;
                result.at(i * n * n + j * n + k).rest_viscosity = rest_viscosity;
                result.at(i * n * n + j * n + k).viscosity = rest_viscosity * glm::pow(glm::e<float>(), visc_alpha * (rest_temperature - temperature));
                result.at(i * n * n + j * n + k).temp_change = 0.0f;
                result.at(i * n * n + j * n + k).heat_const = 0.591f / 4181.3f; // 0.00143f;
                result.at(i * n * n + j * n + k).heat_buffer = 0.0f;
            }
        }
    }
    return result;
}
