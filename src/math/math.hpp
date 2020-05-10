#pragma once

#include "types.hpp"

#include "vec3.hpp"
#include "point3.hpp"

namespace TK {
    // Constants
    static constexpr tkFloat MaxFloat = std::numeric_limits<tkFloat>::max();
    static constexpr tkFloat Infinity = std::numeric_limits<tkFloat>::infinity();

    // Explicit casts
    template <typename T>
    inline Vec3<T>::operator Point3<T>() const {
        return Point3<T>(x, y, z);
    }
    template <typename T>
    inline Point3<T>::operator Vec3<T>() const {
        return Vec3<T>(x, y, z);
    }

    // Vector-Point operations
    template <typename T>
    inline Point3<T> operator+(const Point3<T>& p, const Vec3<T>& v) {
        return Point3<T>(p.x + v.x, p.y + v.y, p.z + v.z);
    }
    template <typename T>
    inline Point3<T> operator-(const Point3<T>& p, const Vec3<T>& v) {
        return Point3<T>(p.x - v.x, p.y - v.y, p.z - v.z);
    }

    // Others
    template <typename T, typename U>
    inline T lerp(T a, T b, U t) {
        return (static_cast<U>(1) - t) * a + t * b;
    }
} // namespace TK
