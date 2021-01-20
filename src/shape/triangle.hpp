#pragma once

#include "core/shape.hpp"

namespace TK {
    class Triangle : public Shape {
    public:
        Triangle(std::shared_ptr<Mesh> mesh, int64_t triIndex, bool invertNormals = false);

        AABBf objectBoundingBox() const override;

        AABBf worldBoundingBox() const override;

        tkFloat surfaceArea() const override;

        bool intersect(const Ray& r, tkFloat& out_tHit, SurfaceInteraction& out_its) const override;

        bool hasIntersect(const Ray& r) const override;

        SurfaceInteraction sample(const Interaction& ref, const Vec2f& u, tkFloat& out_pdf) const override;

    private:
        std::shared_ptr<Mesh> m_Mesh;
        int64_t* m_Id;

    };
}  // namespace TK
