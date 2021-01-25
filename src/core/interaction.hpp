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
        SurfaceInteraction() = default;

        SurfaceInteraction(const Point3f& p, const Vec3f& n, const Vec3f& dpdu, const Vec3f& dpdv,
                           const Vec3f& wo);

        Point3f offsetRayOrigin(const Point3f& o, const Vec3f& n) const;

        Ray spawnRayTo(const Vec3f& d) const override;

        Ray spawnRayTo(const Interaction& ref) const override;

        BSDF getBSDF() const;

        tkSpectrum Le() const;

        Vec3f n;
        Vec3f dpdu;
        Vec3f dpdv;
        Vec3f wo;
        std::shared_ptr<Material> material;
        std::shared_ptr<Light> light;
    };
}  // namespace TK
