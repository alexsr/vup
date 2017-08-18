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
#include "vup/Core/vup.h"
#include <vup/Core/gl_utils.h>
#include "vup/Utility/File_loader.h"

namespace vup
{
    class Shader {
    public:
        Shader(std::string path, GLenum type);
        ~Shader();
        GLuint get_id();
        std::string get_path();
        void reload();

    private:
        void load_shader(const std::string& path);
        std::string m_path;
        GLenum m_type;
        GLuint m_shader_id;
    };
}

#endif //VUP_SHADER_H
