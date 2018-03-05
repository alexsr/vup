//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "OpenGL_debug_logger.h"

vup::OpenGL_debug_logger::OpenGL_debug_logger(GLenum source, GLenum type,
                                              GLenum severity)
    : m_count(1) {
    GLint context_flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
    if ((context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) != 0) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        enable_messages(source, type, severity);
    }
    else {
        std::cout << "Debugging is not enabled." << std::endl;
    }
}

void vup::OpenGL_debug_logger::retrieve_log(std::ostream& out) {
    GLint msg_count = 0;
    glGetIntegerv(GL_DEBUG_LOGGED_MESSAGES, &msg_count);
    GLint max_msg_length = 0;
    glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &max_msg_length);
    std::vector<GLchar> message_data(static_cast<unsigned long>(msg_count * max_msg_length));
    std::vector<GLenum> sources(static_cast<unsigned long>(msg_count));
    std::vector<GLenum> types(static_cast<unsigned long>(msg_count));
    std::vector<GLenum> severities(static_cast<unsigned long>(msg_count));
    std::vector<GLuint> ids(static_cast<unsigned long>(msg_count));
    std::vector<GLsizei> lengths(static_cast<unsigned long>(msg_count));
    GLuint actual_msg_count = glGetDebugMessageLog(static_cast<GLuint>(msg_count),
                                                   static_cast<GLsizei>(message_data.size()),
                                                   sources.data(), types.data(), ids.data(),
                                                   severities.data(), lengths.data(), message_data.data());
    auto data_it = message_data.begin();
    for (unsigned int i = 0; i < actual_msg_count; i++) {
        print_msg(out, sources.at(i), types.at(i), ids.at(i), severities.at(i),
                  std::string(data_it, data_it + lengths.at(i) - 1));
        data_it = data_it + lengths.at(i);
        m_count++;
    }
}

void vup::OpenGL_debug_logger::print_msg(std::ostream& out, GLenum source, GLenum type, GLuint id,
                                         GLenum severity, const std::string& message) {
    out << "Debug message (" << id << "): ";
    out << message << "\n";
    out << "Source: ";
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            out << "API\n";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            out << "window system\n";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            out << "shader compiler\n";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            out << "third party\n";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            out << "application\n";
            break;
        default:
            out << "other\n";
            break;
    }
    out << "Type: ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            out << "error\n";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            out << "deprecated behavior\n";
            break;
        case GL_DEBUG_TYPE_MARKER:
            out << "marker\n";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            out << "performance\n";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            out << "undefined behavior\n";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            out << "portability\n";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            out << "push group\n";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            out << "pop group\n";
            break;
        default:
            out << "other\n";
            break;
    }
    out << "Severity: ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_LOW:
            out << "low\n";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            out << "medium\n";
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            out << "high\n";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        default:
            out << "notification\n";
            break;
    }
    out << std::endl;
}

void vup::OpenGL_debug_logger::enable_messages(GLenum source, GLenum type,
                                               GLenum severity) {
    glDebugMessageControl(source, type, severity, 0, nullptr, GL_TRUE);
}

void vup::OpenGL_debug_logger::disable_messages(GLenum source, GLenum type,
                                                GLenum severity) {
    glDebugMessageControl(source, type, severity, 0, nullptr, GL_FALSE);
}
