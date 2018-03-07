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
        enum class storage : GLbitfield {
            dynamic = GL_DYNAMIC_STORAGE_BIT,
            read = GL_MAP_READ_BIT,
            write = GL_MAP_WRITE_BIT,
            persistent_read = GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT,
            persistent_write = GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT,
            coherent_read = GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_READ_BIT,
            coherent_write = GL_MAP_COHERENT_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT,
            client = GL_CLIENT_STORAGE_BIT

        };

        constexpr std::underlying_type_t<storage> to_gl(storage s) {
            return static_cast<std::underlying_type_t<storage>>(s);
        }

        constexpr storage operator|(const storage s1, const storage s2) {
            return static_cast<storage>(to_gl(s1) | to_gl(s2));
        }

        constexpr bool operator&(const storage s1, const storage s2) {
            return (to_gl(s1) & to_gl(s2)) != 0;
        }

        enum class map_access : GLbitfield {
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

        constexpr std::underlying_type_t<map_access> to_gl(map_access s) {
            return static_cast<std::underlying_type_t<map_access>>(s);
        }

        constexpr map_access operator|(const map_access s1, const map_access s2) {
            return static_cast<map_access>(to_gl(s1) | to_gl(s2));
        }

        constexpr bool operator&(const map_access s1, const map_access s2) {
            return (to_gl(s1) & to_gl(s2)) != 0;
        }
    }
}

#endif //VUP_STORAGE_UTILS_H
