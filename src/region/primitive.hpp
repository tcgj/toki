#pragma once

#include "core/region.hpp"

namespace TK {
    class Diffuse;

    class Primitive : public Region {
    public:
        Primitive(const std::shared_ptr<Shape>& shape,
                  const std::shared_ptr<Material>& material = std::make_shared<Diffuse>(0.6),
                  const std::shared_ptr<Light>& light = nullptr)
            : m_Shape(shape), m_Material(material), m_Light(light) {}

        AABBf worldBoundingBox() const override;

        bool intersect(const Ray& r, SurfaceInteraction& out_its) const override;

        bool hasIntersect(const Ray& r) const override;

    private:
        std::shared_ptr<Shape> m_Shape;
        std::shared_ptr<Material> m_Material;
        std::shared_ptr<Light> m_Light;
    };
}  // namespace TK
