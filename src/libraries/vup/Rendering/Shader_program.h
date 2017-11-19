//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_SHADER_PROGRAM_H
#define VUP_SHADER_PROGRAM_H

#include <vup/Rendering/Shader.h>
#include <vup/GPU_Storage/Uniform.h>
#include <vup/GPU_Storage/Storage_buffer.h>
#include <map>
#include <memory>
#include <iostream>
#include <array>

namespace vup
{
    class Shader_program {
    public:
        void use() const;
        virtual void reload() = 0;
        template <typename T>
        void update_ubo(const std::string& name, const T& data);
        void update_uniform(const std::string& name, bool v);
        void update_uniform(const std::string& name, int v);
        void update_uniform(const std::string& name, float v);
        void update_uniform(const std::string& name, double v);
        void update_uniform(const std::string& name, glm::vec2 v);
        void update_uniform(const std::string& name, glm::vec3 v);
        void update_uniform(const std::string& name, glm::vec4 v);
        void update_uniform(const std::string& name, glm::ivec2 v);
        void update_uniform(const std::string& name, glm::ivec3 v);
        void update_uniform(const std::string& name, glm::ivec4 v);
        void update_uniform(const std::string& name, std::vector<glm::vec2> v);
        void update_uniform(const std::string& name, std::vector<glm::vec3> v);
        void update_uniform(const std::string& name, std::vector<glm::vec4> v);
        void update_uniform(const std::string& name, glm::mat2 v);
        void update_uniform(const std::string& name, glm::mat3 v);
        void update_uniform(const std::string& name, glm::mat4 v);
    protected:
        Shader_program();
        ~Shader_program();
        void link_program() const;
        void init_shader_program();
        void inspect_uniforms();
        void add_uniform(const std::string& name, GLint type, GLint location);
        void inspect_uniform_blocks();
        void add_uniform_block(const std::string& name, GLuint binding, unsigned int size);
        template <typename T>
        bool find_map_entry(const std::string& name,
                            const std::map<std::string, T>& m);
        virtual void attach_shaders() const = 0;
        virtual void detach_shaders() const = 0;
        void clear_maps();
        GLuint m_program_id;
        std::map<std::string, vup::UBO> m_ubos;
        std::map<std::string, Uniform<int>> m_int_uniforms;
        std::map<std::string, Uniform<glm::ivec2>> m_ivec2_uniforms;
        std::map<std::string, Uniform<glm::ivec3>> m_ivec3_uniforms;
        std::map<std::string, Uniform<glm::ivec4>> m_ivec4_uniforms;
        std::map<std::string, Uniform<float>> m_float_uniforms;
        std::map<std::string, Uniform<glm::vec2>> m_vec2_uniforms;
        std::map<std::string, Uniform<glm::vec3>> m_vec3_uniforms;
        std::map<std::string, Uniform<glm::vec4>> m_vec4_uniforms;
        std::map<std::string, Uniform<glm::mat2>> m_mat2_uniforms;
        std::map<std::string, Uniform<glm::mat3>> m_mat3_uniforms;
        std::map<std::string, Uniform<glm::mat4>> m_mat4_uniforms;
    };


    template<typename T>
    void vup::Shader_program::update_ubo(const std::string& name, const T& data) {
        if (find_map_entry(name, m_ubos)) {
            m_ubos.at(name).update_data(data);
        }
    }

    template<typename T>
    bool vup::Shader_program::find_map_entry(const std::string& name,
                                             const std::map<std::string, T>& m) {
        return m.end() != m.find(name);
    }
}


#endif //VUP_SHADER_PROGRAM_H
