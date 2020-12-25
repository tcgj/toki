#pragma once

#include "core/bxdf.hpp"
#include "core/fresnel.hpp"

namespace TK {
    class SpecularReflection : public BxDF {
    public:
        SpecularReflection(const tkSpectrum& dhr, Fresnel* fresnel)
            : BxDF(BxDFType(BXDF_REFLECTIVE | BXDF_SPECULAR)), m_Dhr(dhr), m_Fresnel(fresnel) {}

        tkSpectrum evaluate(const Vec3f& wo, const Vec3f& wi) const override;

        tkSpectrum sample(const Vec3f& wo, Vec3f* wi, const Vec2f& samp, tkFloat* pdf) const override;

        tkFloat getPdf(const Vec3f& wo, const Vec3f& wi) const override;

    private:
        tkSpectrum m_Dhr;  // directional-hemispherical reflectance
        Fresnel* m_Fresnel;
    };
}  // namespace TK
