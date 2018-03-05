//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Window.h"

// The static variable next_id has to be initialized.
int vup::Window::next_id = 0;

vup::Window::Window(const int width, const int height, const std::string& title, const bool debug,
                    const int gl_major, const int gl_minor, GLFWmonitor* monitor,
                    GLFWwindow* share, const int swap_interval)
    : m_id(next_id++), m_width(width), m_height(height) {
    if (debug) {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // enable OpenGL debugging
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
    // Since GLEW has to be initialized separately for each OpenGL context, the method is called here.
    gl::init_GLEW();
}

void vup::Window::make_current() const {
    glfwMakeContextCurrent(m_window);
}

bool vup::Window::should_close() const {
    return glfwWindowShouldClose(m_window) == 1;
}

void vup::Window::swap_buffer() const {
    glfwSwapBuffers(m_window);
}

void vup::Window::run_loop_fixed(const float dt, const std::function<void(float)>& loop) const {
    while (!should_close()) {
        step_loop_fixed(dt, loop);
    }
}

void vup::Window::step_loop_fixed(const float dt, const std::function<void(float)>& loop) const {
    gl::clear_buffers();
    loop(dt);
    swap_buffer();
    glfwPollEvents();
}

int vup::Window::get_id() const {
    return m_id;
}

void vup::Window::resize(const int w, const int h) {
    glfwSetWindowSize(m_window, w, h);
    m_width = w;
    m_height = h;
}

void vup::Window::set_resize(GLFWwindowsizefun resize) const {
    glfwSetWindowSizeCallback(m_window, resize);
}

GLFWwindow* vup::Window::get() const {
    return m_window;
}
