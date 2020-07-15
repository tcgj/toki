#pragma once

#include "core/bxdf.hpp"
#include "core/fresnel.hpp"

namespace TK {
    class SpecularTransmission : public BxDF {
    public:
        SpecularTransmission(const tkSpectrum &dht, tkFloat etaA, tkFloat etaB)
            : BxDF(BxDFType(BXDF_TRANSMISSIVE | BXDF_SPECULAR)),
              dht(dht), fresnel(etaA, etaB), etaA(etaA), etaB(etaB) {}

        tkSpectrum evaluate(const tkVec3f &wo, const tkVec3f &wi) const override;
        tkSpectrum sample(const tkVec3f &wo, tkVec3f *wi, const tkVec2f &samp,
                          tkFloat *pdf) const override;
        tkFloat getPdf(const tkVec3f &wo, const tkVec3f &wi) const override;

    private:
        tkSpectrum dht; // directional-hemispherical transmittance
        DielectricFresnel fresnel;
        tkFloat etaA, etaB;
    };
} // namespace TK