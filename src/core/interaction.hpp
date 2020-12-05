#pragma once

#include "system/toki.hpp"

namespace TK {
    struct Interaction {
        Interaction() = default;
        Interaction(const Point3f& p) : p(p) {}
        virtual ~Interaction() = default;

        Ray spawnRayTo(const Vec3f& d) const;
        Ray spawnRayTo(const Interaction& ref) const;

        Point3f p;
    };

    class SurfaceInteraction : public Interaction {
    public:
        void computeScattering(Scattering* s);
        tkSpectrum Le() const;

        Vec3f n;
        Vec3f dpdu;
        Vec3f dpdv;
        Vec3f wo;
        const Shape* shape = nullptr;
        const Primitive* primitive = nullptr;
        Scattering* scattering = nullptr;
    };
}  // namespace TK