//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_CONTEXT_UTILS_H
#define VUP_CONTEXT_UTILS_H

#include "vup.h"
#include <stdexcept>
#include <string>
#include <array>
#include <iostream>

struct Context {
    GLint major_version{};
    GLint minor_version{};
    std::string vendor;
    std::string renderer;
    std::string extended_glsl_version;
    std::string glsl_version;

    //    // Buffer information
    //    GLint max_ubo_bindings;
    //    GLint max_ssbo_bindings;
    //                
    //    // Framebuffer information
    //    GLint max_color_attachments;
    //    GLint max_color_texture_samples;
    //    GLint max_depth_texture_samples;
    //    GLint max_draw_buffers;
    //    GLint max_framebuffer_height;
    //    GLint max_framebuffer_width;

    // Compute information
    std::array<GLint, 3> max_work_group_count{};
    std::array<GLint, 3> max_work_group_size{};
    GLint max_work_group_invocations{};
    GLint max_compute_shared_memory{};

    void retrieve_info() {
        glGetIntegerv(GL_MAJOR_VERSION, &major_version);
        glGetIntegerv(GL_MINOR_VERSION, &minor_version);
        vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        extended_glsl_version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));
        glsl_version = std::to_string(major_version * 100 + minor_version * 10);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &max_work_group_count.at(0));
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &max_work_group_count.at(1));
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &max_work_group_count.at(2));
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &max_work_group_size.at(0));
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &max_work_group_size.at(1));
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &max_work_group_size.at(2));
        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &max_work_group_invocations);
        glGetIntegerv(GL_MAX_COMPUTE_SHARED_MEMORY_SIZE, &max_compute_shared_memory);
    }

    void print_context_info() {
        std::cout << "OpenGL Version: " << major_version << "." << minor_version << "\n";
        std::cout << "Vendor: " << vendor << "\n";
        std::cout << "Renderer: " << renderer << "\n";
        std::cout << "GLSL Version: " << extended_glsl_version << "\n";
        std::cout << "Compute shader info:\n";
        std::cout << " - Max workgroup count: " << max_work_group_count.at(0) << ", "
            << max_work_group_count.at(1) << ", " << max_work_group_count.at(2) << "\n";
        std::cout << " - Max workgroup size: " << max_work_group_size.at(0) << ", "
            << max_work_group_size.at(1) << ", " << max_work_group_size.at(2) << "\n";
        std::cout << " - Max workgroup invocations: " << max_work_group_invocations << "\n";
        std::cout << " - Max shared memory: " << max_compute_shared_memory << "\n";
    }
};

#endif // VUP_CONTEXT_UTILS_H
