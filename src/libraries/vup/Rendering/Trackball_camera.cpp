//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Trackball_camera.h"

vup::Trackball_camera::Trackball_camera(int width, int height, float sens, float r,
                                        float zoom_sens, glm::vec3 center,
                                        float fov, float near, float far)
        : m_width(width), m_height(height), m_sens(sens), m_radius(r), m_zoom_sens(zoom_sens),
          m_center(center), m_fov(glm::radians(fov)), m_near(near), m_far(far) {
    m_theta = glm::pi<float>() / 2.0f;
    m_phi = 0.0f;
    update_view();
    m_projection = glm::perspective(m_fov, m_width / static_cast<float>(m_height),
                                    m_near, m_far);
}

vup::Trackball_camera::~Trackball_camera() = default;

glm::mat4 vup::Trackball_camera::get_view() const {
    return m_view;
}

glm::mat4 vup::Trackball_camera::get_projection() const {
    return m_projection;
}

void vup::Trackball_camera::update(vup::Window window, float dt) {
    double x, y;
    glfwGetCursorPos(window.get(), &x, &y);
    if (glfwGetMouseButton(window.get(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        float changeX = static_cast<float>(x - m_x) * m_sens * dt;
        float changeY = static_cast<float>(y - m_y) * m_sens * dt;
        m_theta -= changeY;
        if (m_theta < 0.01f) {
            m_theta = 0.01f;
        }
        else if (m_theta > glm::pi<float>() - 0.01f) {
            m_theta = glm::pi<float>() - 0.01f;
        }
        m_phi -= changeX;
        if (m_phi < 0) {
            m_phi += 2 * glm::pi<float>();
        }
        else if (m_phi > 2 * glm::pi<float>()) {
            m_phi -= 2 * glm::pi<float>();
        }
        m_x = x;
        m_y = y;
    }
    else {
        m_x = x;
        m_y = y;
    }
    move_camera(window, dt);
    update_view();
    resize();
}

void vup::Trackball_camera::resize() {
    std::array<int, 4> viewport{};
    glGetIntegerv(GL_VIEWPORT, viewport.data());
    if (m_width != viewport.at(2) || m_height != viewport.at(3)) {
        m_width = viewport.at(2);
        m_height = viewport.at(3);
        m_projection = glm::perspective(m_fov, m_width / static_cast<float>(m_height),
                                        m_near, m_far);
    }
}

void vup::Trackball_camera::update_view() {
    glm::vec3 eye = m_center +
               glm::vec3(m_radius * glm::sin(m_theta) * glm::sin(m_phi),
                         m_radius * glm::cos(m_theta),
                         m_radius * glm::sin(m_theta) * glm::cos(m_phi));
    m_view = glm::lookAt(eye, m_center, glm::vec3(0.0f, 1.0f, 0.0f));
}

void vup::Trackball_camera::move_camera(Window window, float dt) {
    if (glfwGetKey(window.get(), GLFW_KEY_UP) == GLFW_PRESS) {
        m_radius -= m_zoom_sens * dt;
    }
    else if (glfwGetKey(window.get(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        m_radius += m_zoom_sens * dt;
    }
    if (m_radius < 0.1f) {
        m_radius = 0.1f;
    }
}
