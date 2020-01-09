#ifndef MATH_HPP
#define MATH_HPP

class Vector3;
class Point3;

#include "math/vec3.hpp"
#include "math/point3.hpp"

// Point-Vector operations
inline Vec3::operator Point3() const {
    return Point3(x, y, z);
}

inline Point3::operator Vec3() const {
    return Vec3(x, y, z);
}

inline Point3 operator+(const Point3& p, const Vec3& v) {
    return Point3(p.x + v.x, p.y + v.y, p.z + v.z);
}

inline Point3 operator-(const Point3& p, const Vec3& v) {
    return Point3(p.x - v.x, p.y - v.y, p.z - v.z);
}

#endif // MATH_HPP