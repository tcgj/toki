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
        virtual tkFloat surfaceArea() const = 0;
        virtual bool intersect(const Ray &r, tkFloat *tHit,
                               SurfaceInteraction *interaction) const = 0;
        virtual bool hasIntersect(const Ray &r) const;
        virtual Interaction sample(const Interaction &ref, const tkVec2f &samp,
                                   tkFloat *pdf) const = 0;
        virtual tkFloat getPdf(const Interaction &ref, const tkVec3f &wi) const;

    protected:
        const Transform *objectToWorld = nullptr;
        bool invertNormals;
    };
} // namespace TK