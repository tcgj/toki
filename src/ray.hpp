#ifndef RAY_HPP
#define RAY_HPP

#include "math/math.hpp"

class Ray {
public:
    Ray() {}
    Ray(const Point3& pt, const Vec3& dir) : origin(pt), direction(dir) {}

    inline Point3 operator()(float t) const { return origin + t * direction; }

    Point3 origin;
    Vec3 direction;
};

#endif // RAY_HPP