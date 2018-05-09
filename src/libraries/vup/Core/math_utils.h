//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//

#ifndef VUP_MATH_UTILS_H
#define VUP_MATH_UTILS_H

namespace vup
{
    namespace math
    {
        inline unsigned int next_pow2(unsigned int v) {
            v--;
            v |= v >> 1;
            v |= v >> 2;
            v |= v >> 4;
            v |= v >> 8;
            v |= v >> 16;
            return v + 1;
        }
    }
}
#endif // VUP_MATH_UTILS_H
