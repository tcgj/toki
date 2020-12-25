#pragma once

#include "system/toki.hpp"

namespace TK {
    class Ray {
    public:
        Ray() : tMax(TK_INFINITY) /*, time(0.0f), medium(nullptr)*/ {}

        Ray(const Point3f& o, const Vec3f& d, tkFloat tMax = TK_INFINITY
            /*tkFloat time = 0.0f, const Medium* medium = nullptr*/)
            : o(o), d(d), tMax(tMax) /*, time(time), medium(medium)*/ {}

        Point3f operator()(tkFloat t) const;

        Point3f o;
        Vec3f d;
        mutable tkFloat tMax;
        // tkFloat time;
        // const Medium* medium;
    };

    inline Point3f Ray::operator()(tkFloat t) const {
        return o + t * d;
    }
}  // namespace TK
