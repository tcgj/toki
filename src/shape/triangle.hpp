#pragma once

#include "core/shape.hpp"

namespace TK {
    class Triangle : public Shape {
    public:
        Triangle(const std::shared_ptr<Mesh> &mesh, tkUInt triIndex,
                 const Transform *objectToWorld, bool invertNormals = false)
            : Shape(objectToWorld, invertNormals), mesh(mesh), triIndex(triIndex) {}

        tkAABBf objectBoundingBox() const override;
        tkAABBf worldBoundingBox() const override;
        bool intersect(const Ray &r, tkFloat *tHit,
                       SurfaceInteraction *interaction) const override;
        bool hasIntersect(const Ray &r) const override;

    private:
        std::shared_ptr<Mesh> mesh;
        tkUInt triIndex;
    };
} // namespace TK