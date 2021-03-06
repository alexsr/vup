//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "Gui_window.h"

vup::Gui_window::Gui_window(const unsigned int width, const unsigned int height, const std::string& title, const bool debug,
                            const int swap_interval, const bool install_callback,
                            const int gl_major, const int gl_minor,
                            GLFWmonitor* monitor, GLFWwindow* share)
    : Window(width, height, title, debug, swap_interval, gl_major, gl_minor, monitor, share) {
    m_gui_context = gui::init_imgui(m_window, install_callback);
}

void vup::Gui_window::run_loop_fixed(const float dt, const std::function<void(float)>& loop) const {
    while (!should_close()) {
        step_loop_fixed(dt, loop);
    }
    gui::shutdown_imgui(m_gui_context);
    glfwTerminate();
}

void vup::Gui_window::step_loop_fixed(const float dt, const std::function<void(float)>& loop) const {
    gui::start_new_frame(m_context.glsl_version);
    glfwPollEvents();
    gl::clear_buffers();
    loop(dt);
    gui::render_draw_data();
    swap_buffer();
}

void vup::Gui_window::step_loop(const std::function<void()>& loop) const {
    gui::start_new_frame(m_context.glsl_version);
    glfwPollEvents();
    gl::clear_buffers();
    loop();
    gui::render_draw_data();
    swap_buffer();
}

void vup::Gui_window::run_loop(const std::function<void()>& loop) const {
    while (!should_close()) {
        step_loop(loop);
    }
    gui::shutdown_imgui(m_gui_context);
    glfwTerminate();
}

bool vup::Gui_window::check_mouse_action(const int button, const int action) const {
    if (!ImGui::IsAnyWindowFocused()) {
        return glfwGetMouseButton(m_window, button) == action;
    }
    return false;
}

bool vup::Gui_window::check_key_action(const int key, const int action) const {
    if (!ImGui::IsAnyWindowFocused()) {
        return glfwGetKey(m_window, key) == action;
    }
    return false;
}
