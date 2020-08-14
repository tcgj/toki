#pragma once

#include "core/shape.hpp"

namespace TK {
    class Triangle : public Shape {
    public:
        Triangle(const Transform *objectToWorld, const std::shared_ptr<Mesh> &mesh,
                 tkI64 triIndex, bool invertNormals = false)
            : Shape(objectToWorld, invertNormals),
              mesh(mesh), triIndex(triIndex) {}

        tkAABBf objectBoundingBox() const override;
        tkAABBf worldBoundingBox() const override;
        tkFloat surfaceArea() const override;
        bool intersect(const Ray &r, tkFloat *tHit,
                       SurfaceInteraction *interaction) const override;
        bool hasIntersect(const Ray &r) const override;
        SurfaceInteraction sample(const Interaction &ref, const tkVec2f &samp,
                                  tkFloat *pdf) const override;

    private:
        std::shared_ptr<Mesh> mesh;
        tkI64 triIndex;
    };
} // namespace TK