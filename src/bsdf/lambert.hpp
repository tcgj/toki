#pragma once

#include "core/bxdf.hpp"
#include "core/fresnel.hpp"

namespace TK {
    class Lambert : public BxDF {
    public:
        Lambert(const tkSpectrum& dhr) : BxDF(BxDFType(BXDF_REFLECTIVE | BXDF_DIFFUSE)), m_Dhr(dhr) {}

        tkSpectrum evaluate(const Vec3f& wo, const Vec3f& wi) const override {
            return m_Dhr * TK_INVPI;
        }

    private:
        tkSpectrum m_Dhr;  // directional-hemispherical reflectance
    };
}  // namespace TK
