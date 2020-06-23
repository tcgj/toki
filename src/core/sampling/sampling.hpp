#pragma once

#include "math/math.hpp"

namespace TK {
    inline tkVec2f concentricDiskSample(tkFloat u, tkFloat v) {
        tkVec2f offset = (tkFloat)2 * tkVec2f(u, v) - tkVec2f(1);
        if (offset == tkVec2f::zero) {
            return tkVec2f::zero;
        }

        tkFloat theta, r;
        if (std::abs(offset.x) > std::abs(offset.y)) {
            r = offset.x;
            theta = TK_PIOVER4 * (offset.y / offset.x);
        } else {
            r = offset.y;
            theta = TK_PIOVER2 - TK_PIOVER4 * (offset.x / offset.y);
        }

        return r * tkVec2f(std::cos(theta), std::sin(theta));
    }

    inline tkVec3f cosHemisphereSample(tkFloat u, tkFloat v) {
        tkVec2f d = concentricDiskSample(u, v);
        tkFloat z = std::sqrt(std::max((tkFloat)0, 1 - d.x * d.x - d.y * d.y));
        return tkVec3f(d, z);
    }
} // namespace TK
