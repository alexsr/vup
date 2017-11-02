//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_WINDOW_H
#define VUP_WINDOW_H

#include <string>
#include <stdexcept>
#include "vup.h"

namespace vup
{
    class Window {
    public:
        Window(int width, int height, const std::string& title, int gl_major = 4,
               int gl_minor = 5, GLFWmonitor* monitor = nullptr,
               GLFWwindow* share = nullptr, int swap_interval = 0);
        void make_current();
        bool should_close();
        void swap_buffer();
        GLFWwindow* get_GLFWwindow();
    private:
        GLFWwindow* m_window;
        int m_width;
        int m_height;
    };
}

#endif //VUP_WINDOW_H
