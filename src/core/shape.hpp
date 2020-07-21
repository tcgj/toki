#pragma once

#include "system/toki.hpp"
#include "aabb.hpp"
#include "transform.hpp"
#include "interaction.hpp"

namespace TK {
    class Shape {
    public:
        Shape(const Transform *objectToWorld, bool invertNormals = false)
            : objectToWorld(objectToWorld), invertNormals(invertNormals) {}
        virtual ~Shape() = default;

        virtual tkAABBf objectBoundingBox() const = 0;
        virtual tkAABBf worldBoundingBox() const;
        virtual bool intersect(const Ray &r, tkFloat *tHit,
                               SurfaceInteraction *interaction) const = 0;
        virtual bool hasIntersect(const Ray &r) const;

    protected:
        const Transform *objectToWorld = nullptr;
        bool invertNormals;
    };

    // Default inefficient intersection test
    inline bool Shape::hasIntersect(const Ray &r) const {
        tkFloat tHit;
        SurfaceInteraction interaction;
        return intersect(r, &tHit, &interaction);
    }

    inline tkAABBf Shape::worldBoundingBox() const {
        return (*objectToWorld)(objectBoundingBox());
    }
} // namespace TK