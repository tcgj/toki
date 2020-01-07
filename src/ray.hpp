#ifndef RAY_HPP
#define RAY_HPP
#include "math/vec3.hpp"

class ray {
public:
    ray() {}
    ray(const vec3& pt, const vec3& dir) { a = pt, b = dir; }

    vec3 origin() const { return a; }
    vec3 direction() const { return b; }
    vec3 point_at(float t) const { return a + t * b; }

    vec3 a;
    vec3 b;
};

#endif