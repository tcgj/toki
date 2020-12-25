#pragma once

#include "core/bxdf.hpp"
#include "core/fresnel.hpp"

namespace TK {
    class SpecularTransmission : public BxDF {
    public:
        SpecularTransmission(const tkSpectrum& dht, tkFloat etaA, tkFloat etaB)
            : BxDF(BxDFType(BXDF_TRANSMISSIVE | BXDF_SPECULAR)),
              m_Dht(dht),
              m_Fresnel(etaA, etaB),
              m_EtaA(etaA),
              m_EtaB(etaB) {}

        tkSpectrum evaluate(const Vec3f& wo, const Vec3f& wi) const override;

        tkSpectrum sample(const Vec3f& wo, Vec3f* wi, const Vec2f& samp, tkFloat* pdf) const override;

        tkFloat getPdf(const Vec3f& wo, const Vec3f& wi) const override;

    private:
        tkSpectrum m_Dht;  // directional-hemispherical transmittance
        DielectricFresnel m_Fresnel;
        tkFloat m_EtaA;
        tkFloat m_EtaB;
    };
}  // namespace TK
