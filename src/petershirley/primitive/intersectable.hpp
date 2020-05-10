#pragma once

#include "ray/ray.hpp"

class Material;

struct RayInteraction {
    float t;
    tkPoint3f point;
    tkVec3f normal;
    Material* matPtr;
};

class Intersectable {
public:
    virtual bool intersects(const Ray& r, float tMin, float tMax, RayInteraction& ri) const = 0;
};
