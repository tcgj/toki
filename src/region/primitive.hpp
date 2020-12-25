#pragma once

#include "core/region.hpp"

namespace TK {
    class Primitive : public Region {
    public:
        Primitive(const std::shared_ptr<Shape>& shape, const std::shared_ptr<Material>& material,
                  const std::shared_ptr<Light>& light = nullptr)
            : m_Shape(shape), m_Material(material), m_Light(light) {}

        AABBf worldBoundingBox() const override;

        bool intersect(const Ray& r, SurfaceInteraction* interaction) const override;

        bool hasIntersect(const Ray& r) const override;

        std::shared_ptr<Material> getMaterial() const;

        std::shared_ptr<Light> getLight() const;

        void computeScattering(BSDF* bsdf) const;

    private:
        std::shared_ptr<Shape> m_Shape;
        std::shared_ptr<Material> m_Material;
        std::shared_ptr<Light> m_Light;
    };
}  // namespace TK
