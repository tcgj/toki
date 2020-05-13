#pragma once

#include "geometry/transform.hpp"
#include "interaction/surfaceinteraction.hpp"

namespace TK {
    class Shape {
    public:
        Shape() : invertNormals(false) {}
        Shape(const Transform *worldTransform, bool invertNormals)
            : worldTransform(worldTransform), invertNormals(invertNormals) {}

        virtual tkAABBf boundingVolume() const = 0;
        virtual bool intersect(const Ray &r, tkFloat *tHit,
                               SurfaceInteraction *interaction) const = 0;
        virtual bool hasIntersect(const Ray &r, tkFloat *tHit) const;

    protected:
        const Transform *worldTransform = nullptr;
        const bool invertNormals;
    };

    // Default inefficient intersection test
    inline bool Shape::hasIntersect(const Ray &r, tkFloat *tHit) const {
        SurfaceInteraction interaction;
        return intersect(r, tHit, &interaction);
    }
} // namespace TK