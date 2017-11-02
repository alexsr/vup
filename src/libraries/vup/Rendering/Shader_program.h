//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_SHADER_PROGRAM_H
#define VUP_SHADER_PROGRAM_H

#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include "vup/Rendering/Shader.h"

namespace vup
{
    class Shader_program {
    public:
        void use() const;
        virtual void reload() const = 0;
        void update_uniform(const GLchar* name, bool b);
        void update_uniform(const GLchar* name, int i);
        void update_uniform(const GLchar* name, float f);
        void update_uniform(const GLchar* name, double d);
        void update_uniform(const GLchar* name, glm::vec2 v);
        void update_uniform(const GLchar* name, glm::vec3 v);
        void update_uniform(const GLchar* name, glm::vec4 v);
        void update_uniform(const GLchar* name, glm::ivec2 v);
        void update_uniform(const GLchar* name, glm::ivec3 v);
        void update_uniform(const GLchar* name, glm::ivec4 v);
        void update_uniform(const GLchar* name, std::vector<glm::vec2> v);
        void update_uniform(const GLchar* name, std::vector<glm::vec3> v);
        void update_uniform(const GLchar* name, std::vector<glm::vec4> v);
        void update_uniform(const GLchar* name, glm::mat2 m);
        void update_uniform(const GLchar* name, glm::mat3 m);
        void update_uniform(const GLchar* name, glm::mat4 m);
    protected:
        Shader_program();
        ~Shader_program();
        void link_program() const;
        GLint find_uniform_location(const GLchar* name);
        virtual void attach_shaders() const = 0;
        virtual void detach_shaders() const = 0;
        GLuint m_program_id;
    };
}


#endif //VUP_SHADER_PROGRAM_H
