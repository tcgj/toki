#pragma once

#include "core/bxdf.hpp"
#include "core/fresnel.hpp"

namespace TK {
    class SpecularReflection : public BxDF {
    public:
        SpecularReflection(const tkSpectrum& dhr, Fresnel* fresnel)
            : BxDF(BxDFType(BXDF_REFLECTIVE | BXDF_SPECULAR)), dhr(dhr), fresnel(fresnel) {}

        tkSpectrum evaluate(const Vec3f& wo, const Vec3f& wi) const override;

        tkSpectrum sample(const Vec3f& wo, Vec3f* wi, const Vec2f& samp, tkFloat* pdf) const override;

        tkFloat getPdf(const Vec3f& wo, const Vec3f& wi) const override;

    private:
        tkSpectrum dhr;  // directional-hemispherical reflectance
        Fresnel* fresnel = nullptr;
    };
}  // namespace TK
