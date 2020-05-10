#pragma once

#include "petershirley/primitive/intersectable.hpp"

class Sphere : public Intersectable {
public:
    Sphere() {}
    Sphere(tkPoint3f c, float r, Material* m) : center(c), radius(r), matPtr(m) {}

    bool intersects(const Ray& r, float tMin, float tMax, RayInteraction& ri) const;

    tkPoint3f center;
    float radius;
    Material* matPtr;
};
