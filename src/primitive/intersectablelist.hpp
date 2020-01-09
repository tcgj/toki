#ifndef INTERSECTABLE_LIST_HPP
#define INTERSECTABLE_LIST_HPP

#include "primitive/intersectable.hpp"

class IntersectableList : public Intersectable {
public:
    IntersectableList() {}
    IntersectableList(Intersectable** l, int n) : list(l), size(n) {}

    bool intersects(const Ray& r, float tMin, float tMax, RayInteraction& ri) const;

    Intersectable** list;
    int size;
};

#endif  // INTERSECTABLE_LIST_HPP