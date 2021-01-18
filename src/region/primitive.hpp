#pragma once

#include "core/region.hpp"

namespace TK {
    class Primitive : public Region {
    public:
        Primitive(const std::shared_ptr<Shape>& shape,
                  const std::shared_ptr<Material>& material = nullptr,
                  const std::shared_ptr<Light>& light = nullptr);

        AABBf worldBoundingBox() const override;

        bool intersect(const Ray& r, SurfaceInteraction& out_its) const override;

        bool hasIntersect(const Ray& r) const override;

    private:
        std::shared_ptr<Shape> m_Shape;
        std::shared_ptr<Material> m_Material;
        std::shared_ptr<Light> m_Light;
    };
}  // namespace TK
