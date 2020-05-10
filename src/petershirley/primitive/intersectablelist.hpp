#pragma once

#include "petershirley/primitive/intersectable.hpp"

class IntersectableList : public Intersectable {
public:
    IntersectableList() {}
    IntersectableList(Intersectable** l, int n) : list(l), size(n) {}

    bool intersects(const Ray& r, tkFloat tMin, tkFloat tMax, RayInteraction& ri) const;

    Intersectable** list;
    int size;
};
