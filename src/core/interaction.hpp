#pragma once

#include "system/toki.hpp"
#include "ray.hpp"
#include "region/primitive.hpp"

namespace TK {
    struct Interaction {
        Interaction() = default;
        Interaction(const tkPoint3f &p) : p(p) {}
        virtual ~Interaction() = default;

        virtual Ray spawnRayTo(const tkVec3f &d) const;
        Ray spawnRayTo(const Interaction &ref) const;

        tkPoint3f p;
    };

    class SurfaceInteraction : public Interaction {
    public:
        Ray spawnRayTo(const tkVec3f &d) const override;
        void computeScattering(Scattering *s);

        tkVec3f n;
        tkVec3f t;
        tkVec3f wo;
        const Shape *shape = nullptr;
        const Primitive *primitive = nullptr;
        Scattering *scattering = nullptr;
    };

    inline Ray Interaction::spawnRayTo(const tkVec3f &d) const {
        tkPoint3f o = p + d * static_cast<tkFloat>(TK_EPSILON);
        return Ray(o, d);
    }

    inline Ray Interaction::spawnRayTo(const Interaction &ref) const {
        return spawnRayTo(ref.p - p);
    }

    inline Ray SurfaceInteraction::spawnRayTo(const tkVec3f &d) const {
        tkPoint3f o = p + n * static_cast<tkFloat>(TK_EPSILON);
        return Ray(o, d);
    }

    inline void SurfaceInteraction::computeScattering(Scattering *s) {
        if (primitive != nullptr)
            primitive->computeScattering(s);
        scattering = s;
    }
} // namespace TK