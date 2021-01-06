#pragma once

#include "system/toki.hpp"

namespace TK {
    class Interaction {
    public:
        Interaction() = default;

        Interaction(const Point3f& p) : p(p) {}

        virtual ~Interaction() = default;

        virtual Ray spawnRayTo(const Vec3f& d) const;

        virtual Ray spawnRayTo(const Interaction& ref) const;

        Point3f p;
    };

    class SurfaceInteraction : public Interaction {
    public:
        Point3f offsetRayOrigin(const Point3f& o, const Vec3f& n) const;

        Ray spawnRayTo(const Vec3f& d) const override;

        Ray spawnRayTo(const Interaction& ref) const override;

        void computeScattering(BSDF* s);

        tkSpectrum Le() const;

        Vec3f n;
        Vec3f dpdu;
        Vec3f dpdv;
        Vec3f wo;
        const Shape* shape = nullptr;
        const Primitive* primitive = nullptr;
        BSDF* bsdf = nullptr;
    };
}  // namespace TK
