//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_STORAGE_UTILS_H
#define VUP_STORAGE_UTILS_H

#include <vup/Core/vup.h>

namespace vup
{
    namespace gl
    {
        enum class Storage : GLbitfield {
            dynamic = GL_DYNAMIC_STORAGE_BIT,
            read = GL_MAP_READ_BIT,
            write = GL_MAP_WRITE_BIT,
            persistent_read = GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT,
            persistent_write = GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT,
            coherent_read = GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT,
            coherent_write = GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT,
            client = GL_CLIENT_STORAGE_BIT

        };

        constexpr std::underlying_type_t<Storage> to_gl(Storage s) {
            return static_cast<std::underlying_type_t<Storage>>(s);
        }

        constexpr Storage operator|(Storage s1, Storage s2) {
            return static_cast<Storage>(to_gl(s1) | to_gl(s2));
        }

        constexpr bool operator&(Storage s1, Storage s2) {
            return (to_gl(s1) & to_gl(s2)) != 0;
        }

        enum class Map_access : GLbitfield {
            read = GL_MAP_READ_BIT,
            write = GL_MAP_WRITE_BIT,
            persistent_read = GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT,
            persistent_write = GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT,
            coherent_read = GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT,
            coherent_write = GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT,
            invalidate_buffer = GL_MAP_INVALIDATE_BUFFER_BIT,
            invalidate_range = GL_MAP_INVALIDATE_RANGE_BIT,
            flush = GL_MAP_FLUSH_EXPLICIT_BIT,
            unsynchronized = GL_MAP_UNSYNCHRONIZED_BIT

        };

        constexpr std::underlying_type_t<Map_access> to_gl(Map_access s) {
            return static_cast<std::underlying_type_t<Map_access>>(s);
        }

        constexpr Map_access operator|(Map_access s1, Map_access s2) {
            return static_cast<Map_access>(to_gl(s1) | to_gl(s2));
        }

        constexpr bool operator&(Map_access s1, Map_access s2) {
            return (to_gl(s1) & to_gl(s2)) != 0;
        }
    }
}

#endif //VUP_STORAGE_UTILS_H
