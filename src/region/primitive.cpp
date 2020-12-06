#include "primitive.hpp"

#include "core/aabb.hpp"
#include "core/ray.hpp"
#include "core/shape.hpp"
#include "core/material.hpp"

namespace TK {
    AABBf Primitive::worldBoundingBox() const {
        return shape->worldBoundingBox();
    }

    bool Primitive::intersect(const Ray& r, SurfaceInteraction* interaction) const {
        tkFloat tHit;
        if (!shape->intersect(r, &tHit, interaction))
            return false;

        r.tMax = tHit;
        interaction->primitive = this;
        return true;
    }

    bool Primitive::hasIntersect(const Ray& r) const {
        return shape->hasIntersect(r);
    }

    std::shared_ptr<Material> Primitive::getMaterial() const {
        return material;
    }

    std::shared_ptr<Light> Primitive::getLight() const {
        return light;
    }

    void Primitive::computeScattering(BSDF* bsdf) const {
        if (material != nullptr)
            material->computeScattering(bsdf);
    }
}  // namespace TK