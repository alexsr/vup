//
// Alexander Scheid-Rehder
// alexanderb@scheid-rehder.de
// https://www.alexsr.de
// https://github.com/alexsr
//


#include "bounding_box_utils.h"

vup::vox::Bounds vup::vox::reduce_bounds(const std::vector<Bounds>& bounds) {
    Bounds result;
    for (const auto v : bounds) {
        if (v.min.x < result.min.x) {
            result.min.x = v.min.x;
        }
        if (v.min.y < result.min.y) {
            result.min.y = v.min.y;
        }
        if (v.min.z < result.min.z) {
            result.min.z = v.min.z;
        }
        if (v.max.x > result.max.x) {
            result.max.x = v.max.x;
        }
        if (v.max.y > result.max.y) {
            result.max.y = v.max.y;
        }
        if (v.max.z > result.max.z) {
            result.max.z = v.max.z;
        }
    }
    return result;
}
