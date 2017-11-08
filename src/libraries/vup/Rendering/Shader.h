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
#include "vup/Core/gl_utils.h"
#include "vup/Utility/File_loader.h"

namespace vup
{
    class Shader {
    public:
        const GLuint get_id() const;
        const std::string get_path() const;
        void reload() const;
    protected:
        Shader(const filesystem::path& path, GLenum type);
        ~Shader();
        void load_shader(const filesystem::path& path) const;
        filesystem::path m_path;
        GLenum m_type = 0;
        GLuint m_shader_id;
    };

    class Vertex_shader : public vup::Shader {
    public:
        explicit Vertex_shader(const filesystem::path& path);
    };

    class Control_shader : public vup::Shader {
    public:
        explicit Control_shader(const filesystem::path& path);
    };

    class Evaluation_shader : public vup::Shader {
    public:
        explicit Evaluation_shader(const filesystem::path& path);
    };

    class Geometry_shader : public vup::Shader {
    public:
        explicit Geometry_shader(const filesystem::path& path);
    };

    class Fragment_shader : public vup::Shader {
    public:
        explicit Fragment_shader(const filesystem::path& path);
    };
}

#endif //VUP_SHADER_H
