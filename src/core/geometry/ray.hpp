#pragma once

#include "math.hpp"

namespace TK {
    class Ray {
    public:
        Ray() : tMax(Infinity)/*, time(0.0f), medium(nullptr)*/ {}
        Ray(const tkPoint3f& o, const tkVec3f& d, tkFloat tMax = Infinity
            /*tkFloat time = 0.0f, const Medium* medium = nullptr*/)
            : o(o), d(d), tMax(tMax)/*, time(time), medium(medium)*/ {}

        tkPoint3f operator()(tkFloat t) const {
            return o + t * d;
        }

        tkPoint3f o;
        tkVec3f d;
        mutable tkFloat tMax;
        // tkFloat time;
        // const Medium* medium;
    };
} // namespace TK
