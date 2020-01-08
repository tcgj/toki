#ifndef INTERSECTABLE_HPP
#define INTERSECTABLE_HPP

#include "math/math.hpp"
#include "ray.hpp"

struct HitRecord {
    float t;
    Vec3 point;
    Vec3 normal;
};

class Intersectable {
public:
    virtual bool hit(const Ray& r, float tMin, float tMax, HitRecord& rec) = 0;
};

#endif // INTERSECTABLE_HPP