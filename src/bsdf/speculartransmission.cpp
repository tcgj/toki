#include "speculartransmission.hpp"

#include "util/scatteringutil.hpp"

namespace TK {
    tkSpectrum SpecularTransmission::evaluate(const Vec3f& wo, const Vec3f& wi) const {
        return 0;
    }

    tkSpectrum SpecularTransmission::sample(const Vec3f& wo, Vec3f* wi, const Vec2f& samp,
                                            tkFloat* pdf) const {
        bool exiting = cosTheta(wo) > 0;
        tkFloat etaI = exiting ? etaB : etaA;
        tkFloat etaT = exiting ? etaA : etaB;
        *wi = refract(wo, Vec3f::forward, etaT, etaI);
        if (*wi == Vec3f::zero)
            return 0;

        *pdf = 1;
        return dht * (tkSpectrum(1.0f) - fresnel.evaluate(cosTheta(*wi))) / absCosTheta(*wi);
    }

    tkFloat SpecularTransmission::getPdf(const Vec3f& wo, const Vec3f& wi) const {
        return 0;
    }
}  // namespace TK
