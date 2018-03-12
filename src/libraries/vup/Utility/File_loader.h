//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_FILE_LOADER_H
#define VUP_FILE_LOADER_H

#include <fstream>
#include <string_view>
#include <experimental/filesystem>

namespace filesystem = std::experimental::filesystem;

namespace vup
{
    std::string load_file_str(const filesystem::path& path, std::ios_base::openmode mode = std::ios::in);
    const char* load_file_cstr(const filesystem::path& path, std::ios_base::openmode mode = std::ios::in);
}


#endif //VUP_FILE_LOADER_H
