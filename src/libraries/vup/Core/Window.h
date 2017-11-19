//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_WINDOW_H
#define VUP_WINDOW_H

#include "vup.h"
#include "gl_utils.h"
#include <stdexcept>
#include <functional>
#include <string>

namespace vup
{
    class Window {
    public:
        Window(int width, int height, const std::string& title, bool debug = false,
               int gl_major = 4, int gl_minor = 5, GLFWmonitor* monitor = nullptr,
               GLFWwindow* share = nullptr, int swap_interval = 0);
        void make_current();
        bool should_close();
        void swap_buffer();
        void run_loop_fixed(float dt, std::function<void(float)> loop);
        void step_loop_fixed(float dt, std::function<void(float)> loop);
        int get_id();
        GLFWwindow* get();
    private:
        static int next_id;
        int m_id;
        GLFWwindow* m_window;
        int m_width;
        int m_height;
    };
}

#endif //VUP_WINDOW_H
