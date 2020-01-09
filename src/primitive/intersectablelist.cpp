#include "intersectablelist.hpp"

bool IntersectableList::intersects(const Ray& r, float tMin, float tMax, RayInteraction& ri) const {
    bool anyIntersection = false;
    float nearestT = tMax;
    RayInteraction curr;

    for (int i = 0; i < size; i++) {
        if (list[i]->intersects(r, tMin, nearestT, curr)) {
            anyIntersection = true;
            nearestT = curr.t;
            ri = curr;
        }
    }
    return anyIntersection;
}
