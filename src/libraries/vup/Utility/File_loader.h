//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_FILE_LOADER_H
#define VUP_FILE_LOADER_H

#include <string>
#include <fstream>

namespace vup
{
    class File_loader {
    public:
        explicit File_loader(const std::string& path);
        void load(const std::string& path);
        std::string& get_source();
        unsigned long get_size();
    private:
        std::string m_source{""};
    };
}


#endif //VUP_FILE_LOADER_H
