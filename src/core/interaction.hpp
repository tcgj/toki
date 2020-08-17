#pragma once

#include "system/toki.hpp"
#include "math/math.hpp"

namespace TK {
    struct Interaction {
        Interaction() = default;
        Interaction(const tkPoint3f &p) : p(p) {}
        virtual ~Interaction() = default;

        Ray spawnRayTo(const tkVec3f &d) const;
        Ray spawnRayTo(const Interaction &ref) const;

        tkPoint3f p;
    };

    class SurfaceInteraction : public Interaction {
    public:
        void computeScattering(Scattering *s);
        tkSpectrum Le() const;

        tkVec3f n;
        tkVec3f dpdu;
        tkVec3f dpdv;
        tkVec3f wo;
        const Shape *shape = nullptr;
        const Primitive *primitive = nullptr;
        Scattering *scattering = nullptr;
    };
} // namespace TK