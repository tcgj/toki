#include "area.hpp"

namespace TK {
    bool AreaLight::isDelta() const {
        return false;
    }

    tkSpectrum AreaLight::power() const {
        return m_Radiance * m_Area * TK_PI;
    }

    tkSpectrum AreaLight::Le(const SurfaceInteraction& interaction, const Vec3f& wo) const {
        if (dot(interaction.n, wo) <= 0)
            return 0;

        return m_Radiance;
    }

    tkSpectrum AreaLight::sample(const Interaction& ref, Vec3f* wi, const Vec2f& samp, tkFloat* pdf,
                                 OcclusionChecker* occCheck) const {
        SurfaceInteraction samplePt = m_Shape->sample(ref, samp, pdf);
        *wi = -samplePt.wo;
        *occCheck = OcclusionChecker(samplePt, ref);

        return Le(samplePt, samplePt.wo);
    }

    tkFloat AreaLight::getPdf(const Interaction& ref, const Vec3f& wi) const {
        return m_Shape->getPdf(ref, wi);
    }
}  // namespace TK
