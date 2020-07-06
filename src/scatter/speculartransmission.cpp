#include "speculartransmission.hpp"

#include "util/scatteringutil.hpp"

namespace TK {
    tkSpectrum SpecularTransmission::operator()(const tkVec3f &wo, const tkVec3f &wi) const {
        bool entering = cosTheta(wi) > 0;
        tkFloat etaI = entering ? etaA : etaB;
        tkFloat etaT = entering ? etaB : etaA;
        tkVec3f refr = refract(wi, tkVec3f::forward, etaI, etaT);
        if (refr == tkVec3f::zero || dot(refr, wo) < TK_ONE_MINUS_EPSILON)
            return 0;

        return dht * (tkSpectrum(1.0f) - fresnel.evaluate(cosTheta(wi))) / absCosTheta(wi);
    }

    tkSpectrum SpecularTransmission::sample(const tkVec3f &wo, tkVec3f *wi,
                                            const tkVec2f &samp, tkFloat *pdf) const {
        bool exiting = cosTheta(wo) > 0;
        tkFloat etaI = exiting ? etaB : etaA;
        tkFloat etaT = exiting ? etaA : etaB;
        *wi = refract(wo, tkVec3f::forward, etaT, etaI);
        if (*wi == tkVec3f::zero)
            return 0;

        *pdf = 1;
        return dht * (tkSpectrum(1.0f) - fresnel.evaluate(cosTheta(*wi))) / absCosTheta(*wi);
    }
} // namespace TK
