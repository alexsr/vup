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

namespace vup
{
    inline void init_GLFW() {
        int glfw_error = glfwInit();
        if (glfw_error == 0) {
            throw std::runtime_error{ "Failed to initialize GLFW." };
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
        Window(int width, int height, const std::string& title, bool debug = false,
               int gl_major = 4, int gl_minor = 5, GLFWmonitor* monitor = nullptr,
               GLFWwindow* share = nullptr, int swap_interval = 0);
        // Set the OpenGL context of this window to current.
        void make_current() const;
        bool should_close() const;
        void swap_buffer() const;
        // Run a loop with a fixed time step.
        void run_loop_fixed(float dt, const std::function<void(float)>& loop) const;
        // Run a single step of loop with a fixed time step.
        // This method already clears buffers, swaps buffers, and polls events.
        void step_loop_fixed(float dt, const std::function<void(float)>& loop) const;
        void resize(int w, int h);
        // Set the resize function which GLFW should use.
        void set_resize(GLFWwindowsizefun resize) const;
        // Get the unique id of this window.
        int get_id() const;
        GLFWwindow* get() const;
        Context get_context() const;
    private:
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
