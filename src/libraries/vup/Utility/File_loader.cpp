//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "File_loader.h"

std::string vup::load_file_str(const filesystem::path& path, std::ios_base::openmode mode) {
    if (!filesystem::exists(path)) {
        throw std::runtime_error{"File at " + path.string() + " not found."};
    }
    if (filesystem::is_empty(path)) {
        throw std::runtime_error{"File at " + path.string() + " is empty."};
    }
    std::ifstream in;
    in.exceptions(std::ifstream::badbit);
    in.open(path, mode);
    if (!in.is_open()) {
        throw std::runtime_error{"Error loading file from " + path.string()};
    }
    const auto size = filesystem::file_size(path);
    std::string buffer(size, ' ');
    in.read(buffer.data(), size);
    return buffer;
}

const char* vup::load_file_cstr(const filesystem::path& path, std::ios_base::openmode mode) {
    if (!filesystem::exists(path)) {
        throw std::runtime_error{ "File at " + path.string() + " not found." };
    }
    if (filesystem::is_empty(path)) {
        throw std::runtime_error{ "File at " + path.string() + " is empty." };
    }
    std::ifstream in;
    in.exceptions(std::ifstream::badbit);
    in.open(path, mode);
    if (!in.is_open()) {
        throw std::runtime_error{ "Error loading file from " + path.string() };
    }
    const auto size = filesystem::file_size(path);
    std::string buffer(size, ' ');
    in.read(buffer.data(), size);
    return buffer.c_str();
}
