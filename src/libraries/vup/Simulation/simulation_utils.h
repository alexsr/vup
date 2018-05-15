//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_SIMULATION_UTILS_H
#define VUP_SIMULATION_UTILS_H
#include <vup/Core/vup.h>

namespace vup
{
    struct Simulation_timer {
        float calc_dt_average();
        float average_ips();
        float current_ips();
        void update_dt_cfl(float lambda, float diameter, float max_velocity);
        void advance();
        void advance_accumulator(float delta_time);
        bool is_iteration_due();
        float scaled_dt() {
            return time_scaling * dt;
        }
        float inverse_scaled_dt() {
            if (time_scaling == 0) {
                return 0.0f;
            }
            return dt / time_scaling;
        }
        int iterations = 0;
        float time = 0.0f;
        float dt = 0.001f;
        float max_dt = 0.01f;
        float time_scaling = 1.0f;
        float accumulator = 0.0f;
    };

    inline float Simulation_timer::calc_dt_average() {
        return time / iterations;
    }

    inline float Simulation_timer::average_ips() {
        return iterations / time;
    }

    inline float Simulation_timer::current_ips() {
        return 1.0f / inverse_scaled_dt();
    }

    inline void Simulation_timer::update_dt_cfl(const float lambda, const float diameter, const float max_velocity) {
        dt = lambda * diameter / max_velocity;
        if (max_dt < dt) {
            dt = max_dt;
        }
    }

    inline void Simulation_timer::advance_accumulator(float delta_time) {
        accumulator += delta_time;
    }

    inline void Simulation_timer::advance() {
        iterations++;
        time += inverse_scaled_dt();
        accumulator -= inverse_scaled_dt();
    }

    inline bool Simulation_timer::is_iteration_due() {
        return accumulator >= dt;
    }
}

#endif //VUP_SIMULATION_UTILS_H
