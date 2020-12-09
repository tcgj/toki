#pragma once

#include "core/bxdf.hpp"
#include "core/fresnel.hpp"

namespace TK {
    class SpecularTransmission : public BxDF {
    public:
        SpecularTransmission(const tkSpectrum& dht, tkFloat etaA, tkFloat etaB)
            : BxDF(BxDFType(BXDF_TRANSMISSIVE | BXDF_SPECULAR)),
              dht(dht),
              fresnel(etaA, etaB),
              etaA(etaA),
              etaB(etaB) {}

        tkSpectrum evaluate(const Vec3f& wo, const Vec3f& wi) const override;

        tkSpectrum sample(const Vec3f& wo, Vec3f* wi, const Vec2f& samp, tkFloat* pdf) const override;

        tkFloat getPdf(const Vec3f& wo, const Vec3f& wi) const override;

    private:
        tkSpectrum dht;  // directional-hemispherical transmittance
        DielectricFresnel fresnel;
        tkFloat etaA;
        tkFloat etaB;
    };
}  // namespace TK
