//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Texture.h"

vup::Texture::Texture(vup::FBO_tex_desc t, int width, int height) {
    glGenTextures(1, &m_id);
    m_target = t.target;
    m_format = t.format;
    m_sized_format = t.sized_format;
    m_type = t.type;
    resize(width, height);
}

void vup::Texture::resize(int width, int height) {
    m_width = width;
    m_height = height;
    glBindTexture(m_target, m_id);
    glTexImage2D(m_target, 0, m_sized_format, width, height, 0,
                 tex::to_gl(m_format), tex::to_gl(m_type), nullptr);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLuint vup::Texture::get_id() {
    return m_id;
}

GLenum vup::Texture::get_target() {
    return m_target;
}

GLuint vup::Texture::activate(GLuint unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(m_target, m_id);
    return unit;
}