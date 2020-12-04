#pragma once

#include "core/region.hpp"

namespace TK {
    class Primitive : public Region {
    public:
        Primitive(const std::shared_ptr<Shape>& shape, const std::shared_ptr<Material>& material,
                  const std::shared_ptr<Light>& light = nullptr)
            : shape(shape), material(material), light(light) {}

        AABBf worldBoundingBox() const override;
        bool intersect(const Ray& r, SurfaceInteraction* interaction) const override;
        bool hasIntersect(const Ray& r) const override;
        std::shared_ptr<Material> getMaterial() const;
        std::shared_ptr<Light> getLight() const;
        void computeScattering(Scattering* scattering) const;

    private:
        std::shared_ptr<Shape> shape;
        std::shared_ptr<Material> material;
        std::shared_ptr<Light> light;
    };
}  // namespace TK