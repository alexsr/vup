//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_DEMO_UTILS_H
#define VUP_DEMO_UTILS_H

#include "vup.h"
#include <random>
#include <chrono>

namespace vup
{
    typedef std::chrono::high_resolution_clock high_res_clock;
    struct Time_profiler {
        void start() {
            start_time = high_res_clock::now();
        }
        float profile_event() {
            const std::chrono::duration<float> dt = high_res_clock::now() - start_time;
            count_in_second++;
            accumulate_until_second += dt.count();
            if (accumulate_until_second >= 1.0f) {
                one_second_average = accumulate_until_second / count_in_second;
                count_in_second = 0;
                accumulate_until_second = 0.0f;
            }
            start_time = high_res_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(dt).count();;
        }
        float get_one_second_average() const {
            return one_second_average;
        }
        high_res_clock::time_point start_time = high_res_clock::now();
        int count_in_second = 0;
        float accumulate_until_second = 0;
        float one_second_average = 0.0f;
    };
    struct Time_counter {
        float average_aps() {
            return counter / time;
        }
        float current_aps() {
            return 1.0f / dt.count();
        }
        float delta_time() {
            return dt.count();
        }
        void reset() {
            start = high_res_clock::now();
            counter = 0;
            time = 0.0f;
            count_in_second = 0;
            accumulate_until_second = 0.0f;
        }
        void advance() {
            counter++;
            dt = high_res_clock::now() - start;
            time += delta_time();
            count_in_second++;
            accumulate_until_second += delta_time();
            if (accumulate_until_second >= 1.0f) {
                one_second_average = count_in_second / accumulate_until_second;
                count_in_second = 0;
                accumulate_until_second = 0.0f;
            }
            start = high_res_clock::now();
        }
        int counter = 0;
        float time = 0.0f;
        high_res_clock::time_point start = high_res_clock::now();
        std::chrono::duration<float> dt{};
        int count_in_second = 0;
        float accumulate_until_second = 0;
        float one_second_average = 0.0f;
    };

    inline void init_demo_OpenGL_params() {
        glEnable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    inline std::vector<float> generate_random_float_data(int n, float lower_boundary,
                                                         float upper_boundary) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(lower_boundary, upper_boundary);
        std::vector<float> result(static_cast<unsigned long>(n));
        for (auto& r : result) {
            r = dis(gen);
        }
        return result;
    }

    struct MVP {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        void update(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) {
            model = m;
            view = v;
            projection = p;
        }
    };
}

#endif //VUP_DEMO_UTILS_H
