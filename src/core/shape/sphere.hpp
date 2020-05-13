#pragma once

#include "shape.hpp"
#include "geometry/aabb.hpp"

namespace TK {
    class Sphere : public Shape {
    public:
        Sphere(tkFloat radius, const Transform *worldTransform,
                bool invertNormals = false)
            : Shape(worldTransform, invertNormals), radius(radius) {}

        tkAABBf boundingVolume() const override;
        bool intersect(const Ray &r, tkFloat *tHit,
                        SurfaceInteraction *interaction) const override;
        bool hasIntersect(const Ray &r) const override;

    private:
        tkFloat radius;
    };
} // namespace TK