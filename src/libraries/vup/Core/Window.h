//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_WINDOW_H
#define VUP_WINDOW_H

#include "gl_utils.h"
#include "context_utils.h"
#include <stdexcept>
#include <functional>
#include <string>
#include <vup/Core/imgui_utils.h>


namespace vup
{
    inline void init_glfw() {
        const auto glfw_error = glfwInit();
        if (glfw_error == 0) {
            throw std::runtime_error{"Failed to initialize GLFW."};
        }
    }

    /*
     * The Window class encapsulates GLFWwindow and provides additional methods.
     * Every window has a unique id, which can be used to identify each window in a
     * multi-window setup.
     * Additionally there are methods for the loop that is supposed to run inside the
     * window environment while it is open. This is mainly useful for OpenGL use.
     */
    class Window {
    public:
        virtual ~Window() = default;
        Window(unsigned int width, unsigned int height, const std::string& title, bool debug = false,
               int swap_interval = 0, int gl_major = 4, int gl_minor = 5,
               GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
        // Set the OpenGL context of this window to current.
        void make_current() const;
        bool should_close() const;
        void swap_buffer() const;
        // Run a loop with a fixed time step.
        virtual void run_loop_fixed(float dt, const std::function<void(float)>& loop) const;
        // Run a single step of loop with a fixed time step.
        // This method already clears buffers, swaps buffers, and polls events.
        virtual void step_loop_fixed(float dt, const std::function<void(float)>& loop) const;
        // Run a loop.
        virtual void run_loop(const std::function<void()>& loop) const;
        // This method already clears buffers, swaps buffers, and polls events.
        virtual void step_loop(const std::function<void()>& loop) const;
        void resize(int w, int h);
        // Set the resize function which GLFW should use.
        void set_resize(GLFWwindowsizefun resize) const;
        // Get the unique id of this window.
        int get_id() const;
        GLFWwindow* get() const;
        Context get_context() const;
        glm::dvec2 get_cursor_pos() const;
        glm::vec2 get_resolution() const;
        virtual bool check_mouse_action(int button, int action) const;
        virtual bool check_key_action(int, int) const;
    protected:
        // next_id stores the id of the next window that might be created.
        static int next_id;
        int m_id;
        GLFWwindow* m_window;
        int m_width;
        int m_height;
        Context m_context;
    };
}

#endif //VUP_WINDOW_H
