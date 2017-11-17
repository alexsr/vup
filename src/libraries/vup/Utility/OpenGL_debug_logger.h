//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_OPENGL_DEBUG_LOGGER_H
#define VUP_OPENGL_DEBUG_LOGGER_H

#include <vup/Core/gl_utils.h>
#include <memory>
#include <vector>
#include <iostream>

namespace vup
{
    class OpenGL_debug_logger {
    public:
        explicit OpenGL_debug_logger(GLenum source = GL_DONT_CARE,
                                     GLenum type = GL_DONT_CARE, GLenum severity = GL_DONT_CARE);
        void retrieve_log(std::ostream& out = std::cout);
        void enable_messages(GLenum source, GLenum type = GL_DONT_CARE, GLenum severity = GL_DONT_CARE);
        void disable_messages(GLenum source, GLenum type = GL_DONT_CARE, GLenum severity = GL_DONT_CARE);
    private:
        void print_msg(std::ostream& out, GLenum source, GLenum type, GLuint id,
                       GLenum severity, const std::string& message);
        unsigned int m_count;
    };
}

#endif //VUP_OPENGL_DEBUG_LOGGER_H
