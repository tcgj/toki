#pragma once

#include "system/system.hpp"
#include "math/math.hpp"

namespace TK {
    class Ray {
    public:
        Ray() : tMax(TK_INFINITY)/*, time(0.0f), medium(nullptr)*/ {}
        Ray(const tkPoint3f &o, const tkVec3f &d, tkFloat tMax = TK_INFINITY
            /*tkFloat time = 0.0f, const Medium *medium = nullptr*/)
            : o(o), d(d), tMax(tMax)/*, time(time), medium(medium)*/ {}

        tkPoint3f operator()(tkFloat t) const;

        tkPoint3f o;
        tkVec3f d;
        mutable tkFloat tMax;
        // tkFloat time;
        // const Medium *medium;
    };

    inline tkPoint3f Ray::operator()(tkFloat t) const {
        return o + t * d;
    }
} // namespace TK
