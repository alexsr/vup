//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "FBO.h"

vup::FBO::FBO(int width, int height,
              std::vector<vup::FBO_attachment> textures)
        : m_width(width), m_height(height) {
    glCreateFramebuffers(1, &m_id);
    std::vector<GLenum> attachments;
    bool has_depth_attachment = false;
    GLuint i = 0;
    for (auto t : textures) {
        if (t.format == gl::Tex_format::depth) {
            if (has_depth_attachment) {
                throw std::runtime_error{"FBO " + std::to_string(m_id) + " already has a depth attachment."};
            }
            attach_depth_component(t);
            has_depth_attachment = true;
        }
        else {
            attach_color_component(t, i);
            attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
            i++;
        }
    }
    glNamedFramebufferDrawBuffers(m_id, static_cast<GLsizei>(attachments.size()), attachments.data());
    check_fbo_status();
}

void vup::FBO::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void vup::FBO::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

vup::Texture vup::FBO::get_texture(unsigned long i) {
    return m_textures.at(i);
}

GLuint vup::FBO::attach_color_component(vup::FBO_attachment t, unsigned int i) {
    if (t.as_rbo) {
        auto rb = m_renderbuffers.emplace_back(t, m_width, m_height);
        glNamedFramebufferRenderbuffer(m_id, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, rb.get_id());

    }
    else {
        auto texture = m_textures.emplace_back(t, m_width, m_height);
        glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0 + i, texture.get_id(), 0);
    }
}

GLuint vup::FBO::attach_depth_component(vup::FBO_attachment t) {
    if (t.as_rbo) {
        auto rb = m_renderbuffers.emplace_back(t, m_width, m_height);
        glNamedFramebufferRenderbuffer(m_id, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb.get_id());
    }
    else {
        auto texture = m_textures.emplace_back(t, m_width, m_height);
        glNamedFramebufferTexture(m_id, GL_DEPTH_ATTACHMENT, texture.get_id(), 0);
    }
}

void vup::FBO::check_fbo_status() {
    if (glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error{"Framebuffer " + std::to_string(m_id) + " was not created."};
    }
}
