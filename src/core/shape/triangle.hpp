#pragma once

#include "shape.hpp"
#include "geometry/aabb.hpp"
#include "geometry/mesh.hpp"

namespace TK {
    class Triangle : public Shape {
    public:
        Triangle(const std::shared_ptr<Mesh> &mesh, tkInt triIndex,
                 Transform *worldTransform, bool invertNormals = false)
            : Shape(worldTransform, invertNormals), mesh(mesh), triIndex(triIndex) {}

        tkAABBf objectBoundingBox() const override;
        tkAABBf worldBoundingBox() const override;
        bool intersect(const Ray &r, tkFloat *tHit,
                       SurfaceInteraction *interaction) const override;
        bool hasIntersect(const Ray &r) const override;

    private:
        std::shared_ptr<Mesh> mesh;
        const tkInt triIndex;
    };
} // namespace TK