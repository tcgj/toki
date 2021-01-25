#include "shape.hpp"

namespace TK {
    AABBf Shape::worldBoundingBox() const {
        return (*m_ObjectToWorld)(objectBoundingBox());
    }

    // Default inefficient intersection test
    bool Shape::hasIntersect(const Ray& r) const {
        tkFloat tHit;
        SurfaceInteraction its;
        return intersect(r, tHit, its);
    }

    tkFloat Shape::getPdf(const Interaction& ref, const SurfaceInteraction& surface) const {
        Vec3f dir = ref.p - surface.p;
        tkFloat sampleSqrDist = dir.squaredMagnitude();
        tkFloat cosTheta = dot(surface.wo, surface.n);
        if (sampleSqrDist == 0 || cosTheta <= 0)
            return 0;
        else
            return sampleSqrDist / (surfaceArea() * cosTheta);
    }

    tkFloat Shape::getPdf(const Interaction& ref, const Vec3f& wi) const {
        Ray r = ref.spawnRayTo(wi);
        tkFloat tHit;
        SurfaceInteraction its;
        if (!intersect(r, tHit, its))
            return 0;

        return getPdf(ref, its);
    }
}  // namespace TK
