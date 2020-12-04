#pragma once

#include "system/toki.hpp"
#include "aabb.hpp"
#include "transform.hpp"
#include "interaction.hpp"

namespace TK {
    class Shape {
    public:
        Shape(const Transform* objectToWorld, bool invertNormals = false)
            : objectToWorld(objectToWorld), invertNormals(invertNormals) {}
        virtual ~Shape() = default;

        virtual AABBf objectBoundingBox() const = 0;
        virtual AABBf worldBoundingBox() const;
        virtual tkFloat surfaceArea() const = 0;
        virtual bool intersect(const Ray& r, tkFloat* tHit, SurfaceInteraction* interaction) const = 0;
        virtual bool hasIntersect(const Ray& r) const;
        virtual SurfaceInteraction sample(const Interaction& ref, const Vec2f& samp,
                                          tkFloat* pdf) const = 0;
        // Get per solid angle pdf value of sampling surface point on shape from ref point
        tkFloat getPdf(const Interaction& ref, const SurfaceInteraction& surface) const;
        // Get per solid angle pdf value of sampling from direction wi from ref point
        virtual tkFloat getPdf(const Interaction& ref, const Vec3f& wi) const;

    protected:
        const Transform* objectToWorld = nullptr;
        bool invertNormals;
    };
}  // namespace TK