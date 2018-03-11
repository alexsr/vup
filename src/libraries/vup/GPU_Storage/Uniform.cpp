//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Uniform.h"

template <>
void vup::Uniform<int>::update(const int value) const {
    glUniform1i(m_location, value);
}

template <>
void vup::Uniform<glm::ivec2>::update(const glm::ivec2 value) const {
    glUniform2i(m_location, value.x, value.y);
}

template <>
void vup::Uniform<glm::ivec3>::update(const glm::ivec3 value) const {
    glUniform3i(m_location, value.x, value.y, value.z);
}

template <>
void vup::Uniform<glm::ivec4>::update(const glm::ivec4 value) const {
    glUniform4i(m_location, value.x, value.y, value.z, value.w);
}

template <>
void vup::Uniform<float>::update(const float value) const {
    glUniform1f(m_location, value);
}

template <>
void vup::Uniform<glm::vec2>::update(const glm::vec2 value) const {
    glUniform2f(m_location, value.x, value.y);
}

template <>
void vup::Uniform<glm::vec3>::update(const glm::vec3 value) const {
    glUniform3f(m_location, value.x, value.y, value.z);
}

template <>
void vup::Uniform<glm::vec4>::update(const glm::vec4 value) const {
    glUniform4f(m_location, value.x, value.y, value.z, value.w);
}

template <>
void vup::Uniform<glm::mat2>::update(const glm::mat2 value) const {
    glUniformMatrix2fv(m_location, 1, GL_FALSE, glm::value_ptr(value));
}

template <>
void vup::Uniform<glm::mat3>::update(const glm::mat3 value) const {
    glUniformMatrix3fv(m_location, 1, GL_FALSE, glm::value_ptr(value));
}

template <>
void vup::Uniform<glm::mat4>::update(const glm::mat4 value) const {
    glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(value));
}

template <>
void vup::Uniform<int>::update(const std::vector<int>& values) const {
    glUniform1iv(m_location, static_cast<GLsizei>(values.size()), values.data());
}

template <>
void vup::Uniform<glm::ivec2>::update(const std::vector<glm::ivec2>& values) const {
    glUniform2iv(m_location, static_cast<GLsizei>(values.size()), glm::value_ptr(values.data()[0]));
}

template <>
void vup::Uniform<glm::ivec3>::update(const std::vector<glm::ivec3>& values) const {
    glUniform3iv(m_location, static_cast<GLsizei>(values.size()), glm::value_ptr(values.data()[0]));
}

template <>
void vup::Uniform<glm::ivec4>::update(const std::vector<glm::ivec4>& values) const {
    glUniform4iv(m_location, static_cast<GLsizei>(values.size()), glm::value_ptr(values.data()[0]));
}

template <>
void vup::Uniform<float>::update(const std::vector<float>& values) const {
    glUniform1fv(m_location, static_cast<GLsizei>(values.size()), values.data());
}

template <>
void vup::Uniform<glm::vec2>::update(const std::vector<glm::vec2>& values) const {
    glUniform2fv(m_location, static_cast<GLsizei>(values.size()), glm::value_ptr(values.data()[0]));
}

template <>
void vup::Uniform<glm::vec3>::update(const std::vector<glm::vec3>& values) const {
    glUniform3fv(m_location, static_cast<GLsizei>(values.size()), glm::value_ptr(values.data()[0]));
}

template <>
void vup::Uniform<glm::vec4>::update(const std::vector<glm::vec4>& values) const {
    glUniform4fv(m_location, static_cast<GLsizei>(values.size()), glm::value_ptr(values.data()[0]));
}

template <>
void vup::Uniform<glm::mat2>::update(const std::vector<glm::mat2>& values) const {
    glUniformMatrix2fv(m_location, 1, GL_FALSE, glm::value_ptr(values.data()[0]));
}

template <>
void vup::Uniform<glm::mat3>::update(const std::vector<glm::mat3>& values) const {
    glUniformMatrix3fv(m_location, 1, GL_FALSE, glm::value_ptr(values.data()[0]));
}

template <>
void vup::Uniform<glm::mat4>::update(const std::vector<glm::mat4>& values) const {
    glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(values.data()[0]));
}
