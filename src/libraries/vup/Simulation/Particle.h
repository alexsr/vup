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
        glm::vec4 dii;
        glm::vec4 dij_pj_sum;

        float mass;
        float rest_density;
        float density;
        float number_density;

        float viscosity;
        float density_adv;
        float pressure;
        float last_pressure;

        float aii;
        float temperature;
        float temp_change;
        float heat_const;

        float latent_heat;
        float latent_heat_max;
        float temp_melt;
        float pad1;
    };

    struct DFSPH_heat_particle {
        glm::mat4 precond;
        glm::vec4 residual;
        glm::vec4 p;
        glm::vec4 z;
        glm::vec4 x;
        glm::vec4 vel_diff;
        glm::vec4 pos;
        glm::vec4 vel;
        glm::vec4 accel;

        float mass{};
        float rest_density{};
        float density{};
        float density_adv{};

        float K{};
        float K_v{};
        float alpha{};
        float viscosity{};

        float temperature{};
        float temp_change{};
        float heat_const{};
        float heat_buffer{};
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
                                                                         float visc,
                                                                         float temperature_in_celsius,
                                                                         float max_latent_heat,
                                                                         float temp_melt);

    std::vector<DFSPH_heat_particle> create_DFSPH_heat_particles(float r, float h, float lower, float upper,
                                                                 float rest_density, float viscosity,
                                                                 float temperature, float latent_heat_max);
}

#endif //VUP_PARTICLE_H
