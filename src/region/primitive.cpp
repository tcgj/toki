#include "primitive.hpp"

#include "core/aabb.hpp"
#include "core/ray.hpp"
#include "core/shape.hpp"
#include "core/material.hpp"

namespace TK {
    AABBf Primitive::worldBoundingBox() const {
        return m_Shape->worldBoundingBox();
    }

    bool Primitive::intersect(const Ray& r, SurfaceInteraction& out_its) const {
        tkFloat tHit;
        if (!m_Shape->intersect(r, tHit, out_its))
            return false;

        r.tMax = tHit;
        out_its.primitive = this;
        return true;
    }

    bool Primitive::hasIntersect(const Ray& r) const {
        return m_Shape->hasIntersect(r);
    }

    std::shared_ptr<Material> Primitive::getMaterial() const {
        return m_Material;
    }

    std::shared_ptr<Light> Primitive::getLight() const {
        return m_Light;
    }

    void Primitive::computeScattering(BSDF* bsdf) const {
        if (m_Material != nullptr)
            m_Material->computeScattering(bsdf);
    }
}  // namespace TK
