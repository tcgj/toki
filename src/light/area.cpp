#include "area.hpp"

namespace TK {
    bool AreaLight::isDelta() const {
        return false;
    }

    tkSpectrum AreaLight::power() const {
        return radiance * area * TK_PI;
    }

    tkSpectrum AreaLight::Le(const SurfaceInteraction& interaction, const tkVec3f& wo) const {
        if (dot(interaction.n, wo) <= 0)
            return 0;

        return radiance;
    }

    tkSpectrum AreaLight::sample(const Interaction& ref, tkVec3f* wi, const tkVec2f& samp, tkFloat* pdf,
                                 OcclusionChecker* occCheck) const {
        SurfaceInteraction samplePt = shape->sample(ref, samp, pdf);
        *wi = -samplePt.wo;
        *occCheck = OcclusionChecker(ref, samplePt);

        return Le(samplePt, samplePt.wo);
    }

    tkFloat AreaLight::getPdf(const Interaction& ref, const tkVec3f& wi) const {
        return shape->getPdf(ref, wi);
    }
}  // namespace TK
