#pragma once

#include "system/toki.hpp"
#include "bxdf.hpp"

namespace TK {
    class BSDF {
    public:
        BSDF(const SurfaceInteraction& its, BxDF* bxdf);

        tkSpectrum evaluate(const Vec3f& worldWo, const Vec3f& worldWi) const;

        BSDFSample sample(const Vec3f& worldWo, const Vec2f& u, BxDFType type = BXDF_ALL) const;

        tkFloat getPdf(const Vec3f& worldWo, const Vec3f& worldWi, BxDFType type = BXDF_ALL) const;

        Vec3f worldToLocal(const Vec3f& v) const;

        Vec3f localToWorld(const Vec3f& v) const;

    private:
        BxDF* m_Bxdf;
        Vec3f m_N;
        Vec3f m_T;
        Vec3f m_Bt;
    };
}  // namespace TK
