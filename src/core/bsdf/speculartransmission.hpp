#pragma once

#include "bxdf.hpp"
#include "fresnel.hpp"

namespace TK {
    class SpecularTransmission : public BxDF {
    public:
        SpecularTransmission(const tkSpectrum &dht, tkFloat etaA, tkFloat etaB)
            : BxDF(BxDFType(BXDF_TRANSMISSIVE | BXDF_SPECULAR)),
              dht(dht), fresnel(etaA, etaB), etaA(etaA), etaB(etaB) {}

        tkSpectrum operator()(const tkVec3f &wo, const tkVec3f &wi) const;

        tkSpectrum getSample(const tkVec3f &wo, tkVec3f *wi,
                             tkFloat *pdf) const;

    private:
        tkSpectrum dht; // directional-hemispherical transmittance
        DielectricFresnel fresnel;
        tkFloat etaA, etaB;
    };
} // namespace TK