#include "speculartransmission.hpp"

#include "util/scatteringutil.hpp"

namespace TK {
    tkSpectrum SpecularTransmission::evaluate(const tkVec3f& wo, const tkVec3f& wi) const {
        return 0;
    }

    tkSpectrum SpecularTransmission::sample(const tkVec3f& wo, tkVec3f* wi, const tkVec2f& samp,
                                            tkFloat* pdf) const {
        bool exiting = cosTheta(wo) > 0;
        tkFloat etaI = exiting ? etaB : etaA;
        tkFloat etaT = exiting ? etaA : etaB;
        *wi = refract(wo, tkVec3f::forward, etaT, etaI);
        if (*wi == tkVec3f::zero)
            return 0;

        *pdf = 1;
        return dht * (tkSpectrum(1.0f) - fresnel.evaluate(cosTheta(*wi))) / absCosTheta(*wi);
    }

    tkFloat SpecularTransmission::getPdf(const tkVec3f& wo, const tkVec3f& wi) const {
        return 0;
    }
}  // namespace TK
