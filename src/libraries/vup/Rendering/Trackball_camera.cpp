//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Trackball_camera.h"

vup::Trackball_camera::Trackball_camera(int width, int height, float sens, float r,
        float zoom_sens, glm::vec3 center, float fov, float near, float far) {
    m_sens = sens;
    m_zoom_sens = zoom_sens;
    m_width = width;
    m_height = height;
    m_fov = glm::radians(m_fov);
    m_near = near;
    m_far = far;
    m_center = center;

    m_width = width;
    m_height = height;
    m_oldX = 0.0;
    m_oldY = 0.0;
    m_x = 0.0;
    m_y = 0.0;

    m_theta = glm::pi<float>() / 2.0f;
    m_phi = 0.0;
    m_radius = r;

    m_camera_pos.x = m_center.x + m_radius * glm::sin(m_theta) * glm::sin(m_phi);
    m_camera_pos.y = m_center.y + m_radius * glm::cos(m_theta);
    m_camera_pos.z = m_center.z + m_radius * glm::sin(m_theta) * glm::cos(m_phi);

    m_view = glm::lookAt(m_camera_pos, m_center, glm::vec3(0.0f, 1.0f, 0.0f));
    m_projection = glm::perspective(m_fov,
                                    m_width / static_cast<float>(m_height), m_near, m_far);
}

vup::Trackball_camera::~Trackball_camera() = default;

glm::mat4 vup::Trackball_camera::get_view() {
    return m_view;
}

glm::mat4 vup::Trackball_camera::get_projection() {
    return m_projection;
}

void vup::Trackball_camera::update(GLFWwindow* window, float dt) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!m_lmb_pressed) {
            m_lmb_pressed = true;
            m_oldX = x;
            m_oldY = y;
        }
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        double changeX = (x - m_x) * m_sens * dt;
        double changeY = (y - m_y) * m_sens * dt;

        m_theta -= changeY;
        if (m_theta < 0.01) {
            m_theta = 0.01;
        } else if (m_theta > glm::pi<float>() - 0.01f) {
            m_theta = glm::pi<float>() - 0.01f;
        }

        m_phi -= changeX;
        if (m_phi < 0) {
            m_phi += 2 * glm::pi<float>();
        } else if (m_phi > 2 * glm::pi<float>()) {
            m_phi -= 2 * glm::pi<float>();
        }
        m_x = x;
        m_y = y;
    } else {
        if (m_lmb_pressed) {
            m_lmb_pressed = false;
            glfwSetCursorPos(window, m_oldX, m_oldY);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        m_x = x;
        m_y = y;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        m_radius -= m_zoom_sens * dt;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        m_radius += m_zoom_sens * dt;
    }
    if (m_radius < 0.1f) {
        m_radius = 0.1f;
    }

    m_camera_pos.x = m_center.x + m_radius * glm::sin(m_theta) * glm::sin(m_phi);
    m_camera_pos.y = m_center.y + m_radius * glm::cos(m_theta);
    m_camera_pos.z = m_center.z + m_radius * glm::sin(m_theta) * glm::cos(m_phi);

    m_view = glm::lookAt(m_camera_pos, m_center, glm::vec3(0.0f, 1.0f, 0.0f));

}

void vup::Trackball_camera::resize(int width, int height) {
    m_width = width;
    m_height = height;
    m_projection = glm::perspective(m_fov, m_width / static_cast<float>(m_height), m_near, m_far);
}
