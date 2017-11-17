//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_UNIFORM_H
#define VUP_UNIFORM_H

#include <vup/Core/vup.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace vup
{
    template<typename T>
    class Uniform {
    public:
        explicit Uniform<T>(GLint location);
        void update(const std::vector<T>& value);
    private:
        GLint m_location;
    };

    template<typename T>
    Uniform<T>::Uniform(GLint location) {
        m_location = location;
    }

    template<typename T>
    void Uniform<T>::update(const std::vector<T>& value) {

    }
    template<>
    void Uniform<int>::update(const std::vector<int>& values);
    template<>
    void Uniform<glm::ivec2>::update(const std::vector<glm::ivec2>& values);
    template<>
    void Uniform<glm::ivec3>::update(const std::vector<glm::ivec3>& values);
    template<>
    void Uniform<glm::ivec4>::update(const std::vector<glm::ivec4>& values);
    template<>
    void Uniform<float>::update(const std::vector<float>& values);
    template<>
    void Uniform<glm::vec2>::update(const std::vector<glm::vec2>& values);
    template<>
    void Uniform<glm::vec3>::update(const std::vector<glm::vec3>& values);
    template<>
    void Uniform<glm::vec4>::update(const std::vector<glm::vec4>& values);
    template<>
    void Uniform<glm::mat2>::update(const std::vector<glm::mat2>& values);
    template<>
    void Uniform<glm::mat3>::update(const std::vector<glm::mat3>& values);
    template<>
    void Uniform<glm::mat4>::update(const std::vector<glm::mat4>& values);
};

#endif //VUP_UNIFORM_H
