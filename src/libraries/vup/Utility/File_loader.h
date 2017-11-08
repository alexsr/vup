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
#include <string_view>
#include <experimental/filesystem>

namespace filesystem = std::experimental::filesystem;

namespace vup
{
    class File_loader {
    public:
        explicit File_loader(const filesystem::path& path,
                             std::ios_base::openmode mode = std::ios::in);
        void load(const filesystem::path& path, std::ios_base::openmode mode = std::ios::in);
        const std::string& get_source();
        std::string_view get_source_view();
        unsigned long get_size();
    private:
        std::string m_source{""};
    };
}


#endif //VUP_FILE_LOADER_H
