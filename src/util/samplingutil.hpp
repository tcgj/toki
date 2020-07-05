#pragma once

#include "math/math.hpp"
#include "geometryutil.hpp"

namespace TK {
    inline tkVec2f rejectionDiskSample() {
        tkVec2f v;
        do {
            v.x = 2 * Random::nextFloat() - 1;
            v.y = 2 * Random::nextFloat() - 1;
        } while (v.squaredMagnitude() > 1);
        return v;
    }

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

    inline tkVec2f uniformDiskSample(tkFloat u, tkFloat v) {
        tkFloat r = std::sqrt(u);
        tkFloat theta = 2 * TK_PI * v;
        return tkVec2f(r * std::cos(theta), r * std::sin(theta));
    }

    inline tkVec3f cosHemisphereSample(tkFloat u, tkFloat v) {
        tkVec2f d = concentricDiskSample(u, v);
        tkFloat z = std::sqrt(std::max((tkFloat)0, 1 - d.x * d.x - d.y * d.y));
        return tkVec3f(d, z);
    }

    inline tkVec3f uniformHemisphereSample(tkFloat u, tkFloat v) {
        tkFloat theta = std::acos(u);
        tkFloat phi = 2 * TK_PI * v;
        return polarToVec3(theta, phi);
    }

    inline tkVec3f uniformSphereSample(tkFloat u, tkFloat v) {
        tkFloat theta = std::acos(1 - 2 * u);
        tkFloat phi = 2 * TK_PI * v;
        return polarToVec3(theta, phi);
    }
} // namespace TK
