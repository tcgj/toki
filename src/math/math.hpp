#ifndef MATH_HPP
#define MATH_HPP

#include "math/vec3.hpp"
#include "math/point3.hpp"

inline Point3& Point3::operator+=(const Vec3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}
inline Point3& Point3::operator-=(const Vec3& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

inline Point3 operator+(const Point3& p, const Vec3& v) {
    return Point3(p.x + v.x, p.y + v.y, p.z + v.z);
}

inline Point3 operator-(const Point3& p, const Vec3& v) {
    return Point3(p.x - v.x, p.y - v.y, p.z - v.z);
}

#endif // MATH_HPP