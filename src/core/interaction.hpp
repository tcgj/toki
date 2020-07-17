#pragma once

#include "system/system.hpp"
#include "ray.hpp"
#include "region/primitive.hpp"

namespace TK {
    struct Interaction {
        Interaction() = default;
        Interaction(const tkPoint3f &p, const tkVec3f &n, const tkVec3f &t, const tkVec3f &wo)
            : p(p), n(n), t(t), wo(wo) {}
        Interaction(const tkPoint3f &p) : p(p) {}
        virtual ~Interaction() = default;

        Ray spawnRayTo(const tkVec3f &d) const;
        Ray spawnRayTo(const Interaction &interaction) const;

        tkPoint3f p;
        tkVec3f n;
        tkVec3f t;
        tkVec3f wo;
    };

    class SurfaceInteraction : public Interaction {
    public:
        void computeScattering(Scattering *s);

        const Shape *shape = nullptr;
        const Primitive *primitive = nullptr;
        Scattering *scattering = nullptr;
    };

    inline Ray Interaction::spawnRayTo(const tkVec3f &d) const {
        tkPoint3f o = p + n * static_cast<tkFloat>(TK_EPSILON);
        return Ray(o, d);
    }

    inline Ray Interaction::spawnRayTo(const Interaction &interaction) const {
        return spawnRayTo(interaction.p - p);
    }

    inline void SurfaceInteraction::computeScattering(Scattering *s) {
        if (primitive != nullptr)
            primitive->computeScattering(s);
        scattering = s;
    }
} // namespace TK