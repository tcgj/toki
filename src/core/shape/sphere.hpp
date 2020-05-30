#pragma once

#include "shape.hpp"

namespace TK {
    class Sphere : public Shape {
    public:
        Sphere(tkFloat radius, const Transform *objectToWorld,
                bool invertNormals = false)
            : Shape(objectToWorld, invertNormals), radius(radius) {}

        tkAABBf objectBoundingBox() const override;
        bool intersect(const Ray &r, tkFloat *tHit,
                       SurfaceInteraction *interaction) const override;
        bool hasIntersect(const Ray &r) const override;

    private:
        tkFloat radius;
    };
} // namespace TK