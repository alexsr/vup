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
    template <typename T>
    class Uniform {
    public:
        explicit Uniform<T>(GLint location);
        void update(const T value) const;
        void update(const std::vector<T>& value) const;
    private:
        GLint m_location;
    };

    template <typename T>
    Uniform<T>::Uniform(const GLint location) {
        m_location = location;
    }

    template <typename T>
    void Uniform<T>::update(const T value) const {
    }
    
    template <>
    void Uniform<int>::update(int value) const;
    template <>
    void Uniform<glm::ivec2>::update(glm::ivec2 value) const;
    template <>
    void Uniform<glm::ivec3>::update(glm::ivec3 value) const;
    template <>
    void Uniform<glm::ivec4>::update(glm::ivec4 value) const;
    template <>
    void Uniform<float>::update(float value) const;
    template <>
    void Uniform<glm::vec2>::update(glm::vec2 value) const;
    template <>
    void Uniform<glm::vec3>::update(glm::vec3 value) const;
    template <>
    void Uniform<glm::vec4>::update(glm::vec4 value) const;
    template <>
    void Uniform<glm::mat2>::update(glm::mat2 value) const;
    template <>
    void Uniform<glm::mat3>::update(glm::mat3 value) const;
    template <>
    void Uniform<glm::mat4>::update(glm::mat4 value) const;

    template <typename T>
    void Uniform<T>::update(const std::vector<T>& value) const {
    }

    template <>
    void Uniform<int>::update(const std::vector<int>& values) const;
    template <>
    void Uniform<glm::ivec2>::update(const std::vector<glm::ivec2>& values) const;
    template <>
    void Uniform<glm::ivec3>::update(const std::vector<glm::ivec3>& values) const;
    template <>
    void Uniform<glm::ivec4>::update(const std::vector<glm::ivec4>& values) const;
    template <>
    void Uniform<float>::update(const std::vector<float>& values) const;
    template <>
    void Uniform<glm::vec2>::update(const std::vector<glm::vec2>& values) const;
    template <>
    void Uniform<glm::vec3>::update(const std::vector<glm::vec3>& values) const;
    template <>
    void Uniform<glm::vec4>::update(const std::vector<glm::vec4>& values) const;
    template <>
    void Uniform<glm::mat2>::update(const std::vector<glm::mat2>& values) const;
    template <>
    void Uniform<glm::mat3>::update(const std::vector<glm::mat3>& values) const;
    template <>
    void Uniform<glm::mat4>::update(const std::vector<glm::mat4>& values) const;
};

#endif //VUP_UNIFORM_H
