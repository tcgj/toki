#include "shape.hpp"

namespace TK {
    tkAABBf Shape::worldBoundingBox() const {
        return (*objectToWorld)(objectBoundingBox());
    }

    // Default inefficient intersection test
    bool Shape::hasIntersect(const Ray &r) const {
        tkFloat tHit;
        SurfaceInteraction interaction;
        return intersect(r, &tHit, &interaction);
    }

    tkFloat Shape::getPdf(const Interaction &ref, const tkVec3f &wi) const {
        Ray r = ref.spawnRayTo(wi);
        tkFloat tHit;
        SurfaceInteraction interaction;
        if (!intersect(r, &tHit, &interaction))
            return 0;

        tkFloat cosTheta = std::abs(dot(-wi, interaction.n));
        if (cosTheta == 0)
            return 0;

        // pdf wrt area = 1 / area
        // converted wrt to solid angle by factor of r^2 / cosTheta
        return squaredDistance(ref.p, interaction.p) / (cosTheta * surfaceArea());
    }
}  // namespace TK
