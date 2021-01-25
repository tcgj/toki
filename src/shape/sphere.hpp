#pragma once

#include "core/shape.hpp"

namespace TK {
    class Sphere : public Shape {
    public:
        Sphere(const Transform* objectToWorld, tkFloat radius, bool invertNormals = false)
            : Shape(objectToWorld, invertNormals), m_Radius(radius) {}

        AABBf objectBoundingBox() const override;

        tkFloat surfaceArea() const override;

        bool intersect(const Ray& r, tkFloat& out_tHit, SurfaceInteraction& out_its) const override;

        bool hasIntersect(const Ray& r) const override;

        SurfaceInteraction sample(const Interaction& ref, const Vec2f& u, tkFloat& out_pdf) const override;

        tkFloat getPdf(const Interaction& ref, const Vec3f& wi) const override;

    private:
        tkFloat m_Radius;
    };
}  // namespace TK
