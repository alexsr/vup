//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Texture.h"

vup::Texture::Texture(vup::FBO_attachment t, int width, int height) {
    glGenTextures(1, &m_id);
    m_target = t.target;
    m_format = t.format;
    m_sized_format = t.sized_format;
    m_type = t.type;
    m_min_filter = GL_LINEAR;
    m_mag_filter = GL_LINEAR;
    resize(width, height);
}

void vup::Texture::resize(int width, int height) {
    m_width = width;
    m_height = height;
    glBindTexture(m_target, m_id);
    glTexImage2D(m_target, 0, m_sized_format, width, height, 0,
                 gl::to_gl(m_format), gl::to_gl(m_type), nullptr);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, m_min_filter);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, m_mag_filter);
}

GLuint vup::Texture::get_id() {
    return m_id;
}

GLenum vup::Texture::get_target() {
    return m_target;
}

void vup::Texture::activate(GLuint unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(m_target, m_id);
}

void vup::Texture::bind_to_image(GLuint unit, gl::Img_access access, GLenum format) {
    glBindImageTexture(unit, m_id, 0, GL_FALSE, 0, gl::to_gl(access), format);
}
