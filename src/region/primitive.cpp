#include "primitive.hpp"

#include "core/geometry/aabb.hpp"
#include "core/geometry/ray.hpp"
#include "core/shape.hpp"
#include "core/material.hpp"

namespace TK {
    tkAABBf Primitive::worldBoundingBox() const {
        return shape->worldBoundingBox();
    }

    bool Primitive::intersect(const Ray &r, SurfaceInteraction *interaction) const {
        tkFloat tHit;
        if (!shape->intersect(r, &tHit, interaction))
            return false;

        r.tMax = tHit;
        interaction->primitive = this;
        return true;
    }

    bool Primitive::hasIntersect(const Ray &r) const {
        return shape->hasIntersect(r);
    }

    std::shared_ptr<Material> Primitive::getMaterial() const {
        return material;
    }
} // namespace TK