#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "primitive/intersectable.hpp"

class Sphere : public Intersectable {
public:
    Sphere() {}
    Sphere(Point3 c, float r, Material* m) : center(c), radius(r), matPtr(m) {}

    bool intersects(const Ray& r, float tMin, float tMax, RayInteraction& ri) const;

    Point3 center;
    float radius;
    Material* matPtr;
};

#endif // SPHERE_HPP