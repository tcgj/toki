#include "primitive.hpp"

#include "shape/shape.hpp"
#include "geometry/aabb.hpp"

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
    }

    bool Primitive::hasIntersect(const Ray &r) const {
        return shape->hasIntersect(r);
    }
/*
    Material *Primitive::getMaterial() const {
        return material.get();
    }
*/
    void Primitive::ComputeScatteringFunctions(SurfaceInteraction *interaction/*,
                                            MemoryArena &arena*/) const {
        /*if (material)
            material->ComputeScatteringFunctions(interaction, arena);*/
    }
} // namespace TK