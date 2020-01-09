#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "primitive/intersectable.hpp"

class Sphere : public Intersectable {
public:
    Sphere() {}
    Sphere(Point3 c, float r) : center(c), radius(r) {}

    bool intersects(const Ray& r, float tMin, float tMax, RayInteraction& ri) const;

    Point3 center;
    float radius;
};

#endif // SPHERE_HPP