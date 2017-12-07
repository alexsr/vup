//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Window.h"

int vup::Window::next_id = 0;

vup::Window::Window(int width, int height, const std::string& title, bool debug,
                    int gl_major, int gl_minor, GLFWmonitor* monitor,
                    GLFWwindow* share, int swap_interval)
        : m_id(next_id++), m_width(width), m_height(height) {
    if (debug) {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    }
    m_window = glfwCreateWindow(width, height, title.c_str(), monitor, share);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (m_window == nullptr) {
        glfwTerminate();
        throw std::runtime_error{"Failed to create window " + std::to_string(m_id) + "."};
    }
    make_current();
    glfwSwapInterval(swap_interval);
    vup::gl::init_GLEW();
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

void vup::Window::run_loop_fixed(float dt, std::function<void(float)> loop) {
    while (!should_close()) {
        step_loop_fixed(dt, loop);
    }
}

void vup::Window::step_loop_fixed(float dt, std::function<void(float)> loop) {
    vup::gl::clear_buffers();
    loop(dt);
    swap_buffer();
    glfwPollEvents();
}

int vup::Window::get_id() {
    return m_id;
}

void vup::Window::set_resize(GLFWwindowsizefun resize) {
    glfwSetWindowSizeCallback(m_window, resize);
}

GLFWwindow* vup::Window::get() {
    return m_window;
}
