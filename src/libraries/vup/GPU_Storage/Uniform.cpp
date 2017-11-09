//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Uniform.h"

template<>
void vup::Uniform<int>::update(const std::vector<int>& values) {
    glUniform1iv(m_location, static_cast<GLsizei>(values.size()), values.data());
}
template<>
void vup::Uniform<glm::ivec2>::update(const std::vector<glm::ivec2>& values) {
    glUniform2iv(m_location, static_cast<GLsizei>(values.size()), glm::value_ptr(values.data()[0]));
}
template<>
void vup::Uniform<glm::ivec3>::update(const std::vector<glm::ivec3>& values) {
    glUniform3iv(m_location, static_cast<GLsizei>(values.size()), glm::value_ptr(values.data()[0]));
}
template<>
void vup::Uniform<glm::ivec4>::update(const std::vector<glm::ivec4>& values) {
    glUniform4iv(m_location, static_cast<GLsizei>(values.size()), glm::value_ptr(values.data()[0]));
}
template<>
void vup::Uniform<float>::update(const std::vector<float>& values) {
    glUniform1fv(m_location, static_cast<GLsizei>(values.size()), values.data());
}
template<>
void vup::Uniform<glm::vec2>::update(const std::vector<glm::vec2>& values) {
    glUniform2fv(m_location, static_cast<GLsizei>(values.size()), glm::value_ptr(values.data()[0]));
}
template<>
void vup::Uniform<glm::vec3>::update(const std::vector<glm::vec3>& values) {
    glUniform3fv(m_location, static_cast<GLsizei>(values.size()), glm::value_ptr(values.data()[0]));
}
template<>
void vup::Uniform<glm::vec4>::update(const std::vector<glm::vec4>& values) {
    glUniform4fv(m_location, static_cast<GLsizei>(values.size()), glm::value_ptr(values.data()[0]));
}
template<>
void vup::Uniform<glm::mat2>::update(const std::vector<glm::mat2>& values) {
    glUniformMatrix2fv(m_location, 1, GL_FALSE, glm::value_ptr(values.data()[0]));
}
template<>
void vup::Uniform<glm::mat3>::update(const std::vector<glm::mat3>& values) {
    glUniformMatrix3fv(m_location, 1, GL_FALSE, glm::value_ptr(values.data()[0]));
}
template<>
void vup::Uniform<glm::mat4>::update(const std::vector<glm::mat4>& values) {
    glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(values.data()[0]));
}
