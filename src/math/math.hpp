#pragma once

#include "system/types.hpp"

#include "vec3.hpp"
#include "point3.hpp"

#define TK_PI 3.141592653589793
#define TK_INVPI 0.31830988618
#define TK_INV2PI 0.15915494309
#define TK_INV4PI 0.07957747154
#define TK_PIOVER2 1.57079632679
#define TK_PIOVER4 0.78539816339

#define TK_EPSILON 0.00001
#define TK_ONE_MINUS_EPSILON 0.99999

#define degToRad(x) x * TK_PI / 180.0
#define radToDeg(x) x * 180.0 / TK_PI

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
    inline Point3<T> operator+(const Point3<T> &p, const Vec3<T> &v) {
        return Point3<T>(p.x + v.x, p.y + v.y, p.z + v.z);
    }
    template <typename T>
    inline Point3<T> operator-(const Point3<T> &p, const Vec3<T> &v) {
        return Point3<T>(p.x - v.x, p.y - v.y, p.z - v.z);
    }

    // Others
    template <typename T, typename U>
    inline T lerp(T a, T b, U t) {
        return (static_cast<U>(1) - t) * a + t * b;
    }

    inline bool quadratic(tkFloat a, tkFloat b, tkFloat c, tkFloat *t0, tkFloat *t1) {
        tkF64 discriminant = (tkF64)b * (tkF64)b - 4 * (tkF64)a * (tkF64)c;
        if (discriminant < 0)
            return false;

        tkF64 rootDiscriminant = sqrt(discriminant);
        tkF64 x = -0.5 * (b < 0 ? b - rootDiscriminant : b + rootDiscriminant);
        *t0 = x / a;
        *t1 = c / x;
        if (*t0 > *t1)
            std::swap(*t0, *t1);

        return true;
    }
} // namespace TK
