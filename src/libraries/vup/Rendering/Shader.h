//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_SHADER_H
#define VUP_SHADER_H

#include <string>
#include <vector>
#include <vup/Core/gl_utils.h>
#include "vup/Utility/File_loader.h"

namespace vup
{
    class Shader {
    public:
        GLuint get_id();
        std::string get_path();
        void reload();
    protected:
        Shader(const std::string& path, GLenum type);
        ~Shader();
        void load_shader(const std::string& path);
        std::string m_path;
        GLenum m_type = 0;
        GLuint m_shader_id;
    };

    class Vertex_shader : public vup::Shader {
    public:
        explicit Vertex_shader(const std::string& path);
    };

    class Control_shader : public vup::Shader {
    public:
        explicit Control_shader(const std::string& path);
    };

    class Evaluation_shader : public vup::Shader {
    public:
        explicit Evaluation_shader(const std::string& path);
    };

    class Geometry_shader : public vup::Shader {
    public:
        explicit Geometry_shader(const std::string& path);
    };

    class Fragment_shader : public vup::Shader {
    public:
        explicit Fragment_shader(const std::string& path);
    };
}

#endif //VUP_SHADER_H
