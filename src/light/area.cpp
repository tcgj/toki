#include "area.hpp"

namespace TK {
    bool AreaLight::isDelta() const {
        return false;
    }

    tkSpectrum AreaLight::power() const {
        return m_Radiance * m_Area * TK_PI;
    }

    tkSpectrum AreaLight::Le(const SurfaceInteraction& its, const Vec3f& wo) const {
        if (dot(its.n, wo) <= 0)
            return 0;

        return m_Radiance;
    }

    tkSpectrum AreaLight::sample(const Interaction& ref, const Vec2f& u, Vec3f& out_wi, tkFloat& out_pdf,
                                 OcclusionChecker& out_checker) const {
        SurfaceInteraction samplePt = m_Shape->sample(ref, u, out_pdf);
        out_wi = -samplePt.wo;
        out_checker = OcclusionChecker(samplePt, ref);

        return Le(samplePt, samplePt.wo);
    }

    tkFloat AreaLight::getPdf(const Interaction& ref, const Vec3f& wi) const {
        return m_Shape->getPdf(ref, wi);
    }
}  // namespace TK
