//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Window.h"

// The static variable next_id has to be initialized.
int vup::Window::next_id = 0;

vup::Window::Window(const unsigned int width, const unsigned int height, const std::string& title, const bool debug,
                    const int swap_interval, const int gl_major, const int gl_minor,
                    GLFWmonitor* monitor, GLFWwindow* share)
    : m_id(next_id++), m_width(width), m_height(height) {
    if (m_id == 0) {
        init_glfw();
    }
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
    // Since OpenGL has to be initialized separately for each context, the method is called here.
    gl::init_gl();
    gl::set_viewport(width, height);
    const auto resize_callback = [](GLFWwindow* window, int w, int h) { glViewport(0, 0, w, h); };
    set_resize(resize_callback);
    m_context.retrieve_info();
    m_context.print_context_info();
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
    glfwTerminate();
}

void vup::Window::step_loop_fixed(const float dt, const std::function<void(float)>& loop) const {
    glfwPollEvents();
    gl::clear_buffers();
    loop(dt);
    swap_buffer();
}

void vup::Window::run_loop(const std::function<void()>& loop) const {
    while (!should_close()) {
        step_loop(loop);
    }
    glfwTerminate();
}

void vup::Window::step_loop(const std::function<void()>& loop) const {
    glfwPollEvents();
    gl::clear_buffers();
    loop();
    swap_buffer();
}

int vup::Window::get_id() const {
    return m_id;
}

void vup::Window::resize(const int w, const int h) {
    glfwSetWindowSize(m_window, w, h);
    m_width = w;
    m_height = h;
}

void vup::Window::set_resize(const GLFWwindowsizefun resize) const {
    glfwSetWindowSizeCallback(m_window, resize);
}

GLFWwindow* vup::Window::get() const {
    return m_window;
}

Context vup::Window::get_context() const {
    return m_context;
}

glm::dvec2 vup::Window::get_cursor_pos() const {
    glm::dvec2 pos;
    glfwGetCursorPos(m_window, &pos.x, &pos.y);
    return pos;
}

glm::vec2 vup::Window::get_resolution() const {
    return glm::vec2(m_width, m_height);
}

bool vup::Window::check_mouse_action(const int button, const int action) const {
    return glfwGetMouseButton(m_window, button) == action;
}

bool vup::Window::check_key_action(const int key, const int action) const {
    return glfwGetKey(m_window, key) == action;
}
