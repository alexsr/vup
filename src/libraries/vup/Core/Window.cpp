//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Window.h"

vup::Window::Window(int width, int height, const std::string& title, bool debug,
                    int gl_major, int gl_minor, GLFWmonitor* monitor, GLFWwindow* share,
                    int swap_interval)
        : m_width(width), m_height(height) {
    if (debug) {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    }
    m_window = glfwCreateWindow(width, height, title.c_str(), monitor, share);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (m_window == nullptr) {
        glfwTerminate();
        throw std::runtime_error{"Failed to create window."};
    }
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int w, int h) {
        glViewport(0, 0, w, h);
    });
    make_current();
    glfwSwapInterval(swap_interval);
}

void vup::Window::make_current() {
    glfwMakeContextCurrent(m_window);
}

bool vup::Window::should_close() {
    return glfwWindowShouldClose(m_window) == 1;
}

void vup::Window::swap_buffer() {
    glfwSwapBuffers(m_window);
}

GLFWwindow* vup::Window::get_GLFWwindow() {
    return m_window;
}
