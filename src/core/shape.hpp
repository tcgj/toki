#pragma once

#include "system/toki.hpp"
#include "aabb.hpp"
#include "transform.hpp"
#include "interaction.hpp"

namespace TK {
    class Shape {
    public:
        Shape(const Transform* objectToWorld, bool invertNormals = false)
            : m_ObjectToWorld(objectToWorld), m_InvertNormals(invertNormals) {}

        virtual ~Shape() = default;

        virtual AABBf objectBoundingBox() const = 0;

        virtual AABBf worldBoundingBox() const;

        virtual tkFloat surfaceArea() const = 0;

        virtual bool intersect(const Ray& r, tkFloat& out_tHit, SurfaceInteraction& out_its) const = 0;

        virtual bool hasIntersect(const Ray& r) const;

        virtual SurfaceInteraction sample(const Interaction& ref, const Vec2f& u, tkFloat& out_pdf) const = 0;

        // Get per solid angle pdf value of sampling surface point on shape from ref point
        tkFloat getPdf(const Interaction& ref, const SurfaceInteraction& surface) const;

        // Get per solid angle pdf value of sampling from direction wi from ref point
        virtual tkFloat getPdf(const Interaction& ref, const Vec3f& wi) const;

    protected:
        const Transform* m_ObjectToWorld = nullptr;
        bool m_InvertNormals;
    };
}  // namespace TK
