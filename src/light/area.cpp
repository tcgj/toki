#include "area.hpp"

namespace TK {
    bool AreaLight::isDelta() const {
        return false;
    }

    tkSpectrum AreaLight::power() const {
        return radiance * area * TK_PI;
    }

    tkSpectrum AreaLight::computeLe(const SurfaceInteraction &interaction,
                                    const tkVec3f &wi) const {
        if (dot(interaction.n, wi) <= 0)
            return 0;

        return radiance;
    }

    tkSpectrum AreaLight::sample(const Interaction &ref, tkVec3f *wi,
                                 tkVec2f &samp, tkFloat *pdf,
                                 OcclusionChecker *occCheck) const {
        Interaction samplePt = shape->sample(ref, samp, pdf);
        *wi = normalize(samplePt.p - ref.p);
        *occCheck = OcclusionChecker(ref, samplePt);
        return radiance;
    }

    tkFloat AreaLight::getPdf(const Interaction &ref, const tkVec3f &wi) const {
        return shape->getPdf(ref, wi);
    }
} // namespace TK
