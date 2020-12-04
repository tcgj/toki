#pragma once

#include "core/shape.hpp"

namespace TK {
    class Triangle : public Shape {
    public:
        Triangle(const Transform* objectToWorld, const std::shared_ptr<Mesh>& mesh, int64_t triIndex,
                 bool invertNormals = false)
            : Shape(objectToWorld, invertNormals), mesh(mesh), triIndex(triIndex) {}

        AABBf objectBoundingBox() const override;
        AABBf worldBoundingBox() const override;
        tkFloat surfaceArea() const override;
        bool intersect(const Ray& r, tkFloat* tHit, SurfaceInteraction* interaction) const override;
        bool hasIntersect(const Ray& r) const override;
        SurfaceInteraction sample(const Interaction& ref, const Vec2f& samp, tkFloat* pdf) const override;

    private:
        std::shared_ptr<Mesh> mesh;
        int64_t triIndex;
    };
}  // namespace TK