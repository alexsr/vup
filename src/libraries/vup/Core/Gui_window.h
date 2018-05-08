//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_GUIWINDOW_H
#define VUP_GUIWINDOW_H

#include "Window.h"

namespace vup
{
    /*
     * The Window class encapsulates GLFWwindow and provides additional methods.
     * Every window has a unique id, which can be used to identify each window in a
     * multi-window setup.
     * Additionally there are methods for the loop that is supposed to run inside the
     * window environment while it is open. This is mainly useful for OpenGL use.
     */
    class Gui_window : public Window {
    public:
        Gui_window(unsigned int width, unsigned int height, const std::string& title, bool debug = false,
                   int swap_interval = 0, bool install_callback = true, int gl_major = 4, int gl_minor = 5,
                   GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);
        void run_loop_fixed(float dt, const std::function<void(float)>& loop) const override;
        // Run a single step of loop with a fixed time step.
        // This method already clears buffers, swaps buffers, and polls events.
        void step_loop_fixed(float dt, const std::function<void(float)>& loop) const override;
        // This method already clears buffers, swaps buffers, and polls events.
        void step_loop(const std::function<void()>& loop) const override;
        //Run a loop.
        void run_loop(const std::function<void()>& loop) const override;

        bool check_mouse_action(int button, int action) const override;
        bool check_key_action(int, int) const override;
    private:
        ImGuiContext* m_gui_context;
    };
}

#endif //VUP_GUIWINDOW_H
