#pragma once

#include "region.hpp"

namespace TK {
    class Primitive : public Region {
    public:
        Primitive(const std::shared_ptr<Shape> &shape,
                  const std::shared_ptr<Material> &material)
            : shape(shape), material(material) {}

        tkAABBf worldBoundingBox() const override;
        bool intersect(const Ray &r, SurfaceInteraction *interaction) const override;
        bool hasIntersect(const Ray &r) const override;
        std::shared_ptr<Material> getMaterial() const;

    private:
        std::shared_ptr<Shape> shape;
        std::shared_ptr<Material> material;
    };
} // namespace TK