//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_SHADER_PROGRAM_H
#define VUP_SHADER_PROGRAM_H

#include <vup/Utility/File_loader.h>
#include <vup/GPU_Storage/Uniform.h>
#include <vup/GPU_Storage/Storage_buffer.h>
#include <map>
#include <functional>
#include <iostream>
#include <array>

namespace vup
{
    namespace gl
    {
        inline std::string shader_type_to_string(const GLenum type) {
            switch (type) {
            case GL_VERTEX_SHADER:
                return "vertex shader";
            case GL_FRAGMENT_SHADER:
                return "fragment shader";
            case GL_GEOMETRY_SHADER:
                return "geometry shader";
            case GL_TESS_CONTROL_SHADER:
                return "tessellation control shader";
            case GL_TESS_EVALUATION_SHADER:
                return "tessellation evaluation shader";
            case GL_COMPUTE_SHADER:
                return "compute shader";
            default:
                return "unknown type of shader";
            }
        }

        enum class introspection : GLbitfield {
            none = 0,
            basic = 2
        };

        constexpr std::underlying_type_t<introspection> to_gl(introspection i) {
            return static_cast<std::underlying_type_t<introspection>>(i);
        }

        constexpr introspection operator|(const introspection i1, const introspection i2) {
            return static_cast<introspection>(to_gl(i1) | to_gl(i2));
        }

        constexpr bool operator&(const introspection i1, const introspection i2) {
            return (to_gl(i1) & to_gl(i2)) != 0;
        }
    }
    struct Shader_define {
        std::string name;
        std::string value;
    };

    class Shader {
    public:
        virtual ~Shader() = default;
        void use() const;
        virtual void reload() = 0;
        void update_uniform(const std::string& name, bool v) const;
        void update_uniform(const std::string& name, unsigned int v) const;
        void update_uniform(const std::string& name, int v) const;
        void update_uniform(const std::string& name, float v) const;
        void update_uniform(const std::string& name, double v) const;
        void update_uniform(const std::string& name, glm::vec2 v) const;
        void update_uniform(const std::string& name, glm::vec3 v) const;
        void update_uniform(const std::string& name, glm::vec4 v) const;
        void update_uniform(const std::string& name, glm::ivec2 v) const;
        void update_uniform(const std::string& name, glm::ivec3 v) const;
        void update_uniform(const std::string& name, glm::ivec4 v) const;
        void update_uniform(const std::string& name, const std::vector<glm::vec2>& v) const;
        void update_uniform(const std::string& name, const std::vector<glm::vec3>& v) const;
        void update_uniform(const std::string& name, const std::vector<glm::vec4>& v) const;
        void update_uniform(const std::string& name, glm::mat2 v) const;
        void update_uniform(const std::string& name, glm::mat3 v) const;
        void update_uniform(const std::string& name, glm::mat4 v) const;
    protected:
        explicit Shader(std::vector<Shader_define> defines = {},
                        gl::introspection introspection_flag = gl::introspection::basic);
        GLuint load_shader(const filesystem::path& path, GLenum type);
        void process_includes(std::string& file, const filesystem::path& directory,
                              unsigned long long start, unsigned long long end_search);
        void link_program() const;
        void init_shader_program(const std::vector<GLuint>& shader_ids);
        void attach_shaders(const std::vector<GLuint>& shader_ids) const;
        void detach_shaders(const std::vector<GLuint>& shader_ids) const;
        static void delete_shaders(const std::vector<GLuint>& shader_ids);
        void inspect_uniforms();
        void add_uniform(const std::string& name, GLint type, GLint location);
        template <typename T>
        static bool find_map_entry(const std::string& name,
                                   const std::map<std::string, T>& m);
        void clear_maps();
        GLuint m_program_id;
        gl::introspection m_introspection_flag;
        std::vector<Shader_define> m_defines;
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

    template <typename T>
    bool Shader::find_map_entry(const std::string& name,
                                const std::map<std::string, T>& m) {
        return m.end() != m.find(name);
    }

    class V_F_shader : public Shader {
    public:
        V_F_shader(filesystem::path vertex_path,
                   filesystem::path fragment_path,
                   const std::vector<Shader_define>& defines = {},
                   gl::introspection introspection_flag = gl::introspection::basic);
        virtual ~V_F_shader() = default;
        void reload() override;
    private:
        filesystem::path m_vertex_path;
        filesystem::path m_fragment_path;
    };

    class V_G_F_shader : public Shader {
    public:
        V_G_F_shader(filesystem::path vertex_path,
                     filesystem::path geometry_path,
                     filesystem::path fragment_path,
                     const std::vector<Shader_define>& defines = {},
                     gl::introspection introspection_flag = gl::introspection::basic);
        void reload() override;
    private:
        filesystem::path m_vertex_path;
        filesystem::path m_geometry_path;
        filesystem::path m_fragment_path;
    };

    class V_T_F_shader : public Shader {
    public:
        V_T_F_shader(filesystem::path vertex_path,
                     filesystem::path control_path,
                     filesystem::path evaluation_path,
                     filesystem::path fragment_path,
                     const std::vector<Shader_define>& defines = {},
                     gl::introspection introspection_flag = gl::introspection::basic);
        void reload() override;
    private:
        filesystem::path m_vertex_path;
        filesystem::path m_control_path;
        filesystem::path m_evaluation_path;
        filesystem::path m_fragment_path;
    };

    class V_T_G_F_shader : public Shader {
    public:
        V_T_G_F_shader(filesystem::path vertex_path,
                       filesystem::path control_path,
                       filesystem::path evaluation_path,
                       filesystem::path geometry_path,
                       filesystem::path fragment_path,
                       const std::vector<Shader_define>& defines = {},
                       gl::introspection introspection_flag = gl::introspection::basic);
        void reload() override;
    private:
        filesystem::path m_vertex_path;
        filesystem::path m_control_path;
        filesystem::path m_evaluation_path;
        filesystem::path m_geometry_path;
        filesystem::path m_fragment_path;
    };
}


#endif //VUP_SHADER_PROGRAM_H
