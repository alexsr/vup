//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_COMPUTE_UTILS_H
#define VUP_COMPUTE_UTILS_H

#include <numeric>
#include <utility>
#include <execution>
#include <vup/Utility/File_loader.h>
#include "vup/Shader/Compute_shader.h"
#include "vup/Core/math_utils.h"

namespace vup
{
    class Reduction {
    public:
        Reduction(const filesystem::path& shader_path, const std::shared_ptr<Empty_SSBO>& ssbo, unsigned int count);
        template <typename T, typename Operator>
        T execute(Operator op);
    protected:
        std::vector<Compute_shader> m_shaders{};
        std::vector<unsigned int> m_workgroup_counts{};
        std::shared_ptr<Empty_SSBO> m_ssbo;
        unsigned int m_count;
    };

    inline Reduction::Reduction(const filesystem::path& shader_path, const std::shared_ptr<Empty_SSBO>& ssbo, const unsigned int count)
        : m_ssbo(ssbo), m_count(count) {
        int shader_id = 0;
        while (m_count > 16) {
            const auto max_threads = std::min(math::next_pow2(m_count) / 2, static_cast<unsigned int>(512));
            const auto reduce_instances = static_cast<unsigned int>(glm::ceil(m_count / 2.0f));
            m_count = static_cast<int>(glm::ceil(reduce_instances / max_threads));
            std::vector<Shader_define> defines{
                {"BUFFER_ID", std::to_string(m_ssbo->get_binding())},
                {"X", std::to_string(max_threads)},
                {"N", std::to_string(m_count)}
            };
            m_shaders.emplace_back(shader_path, defines);
            m_shaders.at(shader_id).update_uniform("max_blocks", m_count);
            m_workgroup_counts.push_back(reduce_instances);
            shader_id++;
        }
    }

    template <typename T, typename Operator>
    T Reduction::execute(Operator op) {
        std::vector<T> result(m_count);
        for (unsigned long long i = 0; i < m_shaders.size(); i++) {
            m_shaders.at(i).run_with_barrier(m_workgroup_counts.at(i));
        }
        m_ssbo->get_data(result);
        return std::reduce(std::execution::par, result.begin(), result.end(), 0.0f, op);
    }
}

#endif // VUP_COMPUTE_UTILS_H
