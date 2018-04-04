//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_PARTICLE_H
#define VUP_PARTICLE_H

#include <vup/Core/vup.h>
#include <vector>

namespace vup
{
    struct SPH_particle {
        glm::vec4 pos;
        glm::vec4 vel;
        glm::vec4 acc;
        glm::vec4 force;
        float mass{};
        float rest_density{};
        float pressure{};
        float density{};
    };

    struct IISPH_particle {
        glm::vec4 pos;
        glm::vec4 vel;
        glm::vec4 vel_adv;
        glm::vec4 pressure_force;
        glm::vec4 dii;
        glm::vec4 dij_pj_sum;
        float mass{};
        float rest_density{};
        float density{};
        float density_adv{};
        float pressure{};
        float last_pressure{};
        float aii{};
        float aij_pj_sum{};
    };

    struct IISPH_heat_particle {
        glm::vec4 pos;
        glm::vec4 vel;
        glm::vec4 vel_adv;
        glm::vec4 pressure_force;
        glm::vec4 dii;
        glm::vec4 dij_pj_sum;
        glm::vec4 temperature_grad;
        float mass{};
        float rest_density{};
        float density{};
        float density_adv{};
        float pressure{};
        float last_pressure{};
        float aii{};
        float aij_pj_sum{};
        float temperature{};
        float temp_change{};
        float heat_const{};
        float heat_buffer{};
    };

    struct Phase_change_particle {
        // Standard data 2
        glm::vec4 pos;
        glm::vec4 vel;
        // IISPH 6
        glm::vec4 vel_adv;
        glm::vec4 pressure_accel;
        glm::vec4 dii;
        glm::vec4 dij_pj_sum;
        float mass;
        float rest_density;
        float density;
        float density_adv;
        float pressure;
        float last_pressure;
        float aii;
        float aij_pj_sum;
        // Temperature and phase 3 + 2
        glm::vec4 temperature_grad;
        float temperature;
        float rest_temperature;
        float temp_change;
        float heat_const;
        int phase;
        float t_solidify;
        float t_melt;
        float t_boil;
        float latent_heat_per_mass;
        float heat_buffer;
        // Viscosity 0 + 2
        float viscosity;
        float rest_viscosity;
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

    std::vector<SPH_particle> create_uniform_SPH_particles(float radius, float lower, float upper,
                                                           float rest_density, float h);

    std::vector<IISPH_particle> create_uniform_IISPH_particles(float r, float h, float lower, float upper,
                                                               float rest_density);

    std::vector<IISPH_heat_particle> create_uniform_IISPH_heat_particles(float r, float h, float lower, float upper,
                                                                         float rest_density,
                                                                         float temperature_in_celsius);

    std::vector<Phase_change_particle> create_uniform_Phase_change_particles(float r, float h, float lower, float upper,
                                                                             float rest_density, float temperature,
                                                                             float rest_temperature,
                                                                             float rest_viscosity, float visc_alpha);
}

#endif //VUP_PARTICLE_H
