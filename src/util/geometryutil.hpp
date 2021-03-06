#pragma once

#include "system/toki.hpp"

namespace TK {
    /*
    Returns a normalized vector in the current coordinate
    space representing the polar coordinates (in radians) given.
    */
    inline Vec3f polarToVec3(tkFloat sinTheta, tkFloat cosTheta, tkFloat phi) {
        return Vec3f(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);
    }

    /*
    Overloaded polarToVec3 function.
    Precomputes the cosine and sine values from the given theta.
    */
    inline Vec3f polarToVec3(tkFloat theta, tkFloat phi) {
        return polarToVec3(std::sin(theta), std::cos(theta), phi);
    }

    inline Vec3f polarToVec3(tkFloat sinTheta, tkFloat cosTheta, tkFloat phi, const Vec3f& x,
                               const Vec3f& y, const Vec3f& z) {
        return sinTheta * std::cos(phi) * x + sinTheta * std::sin(phi) * y + cosTheta * z;
    }

    /*
    Returns the angle theta from the z-axis
    given the direction vector v, in radians.
    */
    inline tkFloat vec3ToTheta(const Vec3f& v) {
        return std::acos(clamp(v.z, -1, 1));
    }

    /*
    Returns the angle phi on the xy-plane (from x-axis to y-axis)
    given the direction vector v, in radians.
    */
    inline tkFloat vec3ToPhi(const Vec3f& v) {
        tkFloat angle = std::atan2(v.y, v.x);
        return angle < 0 ? angle + 2 * TK_PI : angle;
    }
}  // namespace TK
