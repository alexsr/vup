//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#include "File_loader.h"

vup::File_loader::File_loader(const std::string& path) {
    load(path);
}

void vup::File_loader::load(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw std::runtime_error{"Error loading file from " + path};
    }
    in.seekg(0, std::ios::end);
    m_source.resize(static_cast<unsigned long>(in.tellg()));
    in.seekg(0, std::ios::beg);
    in.read(&m_source[0], m_source.size());
    in.close();
}

std::string vup::File_loader::get_source() {
    return m_source;
}

unsigned long vup::File_loader::get_size() {
    return m_source.size();
}
