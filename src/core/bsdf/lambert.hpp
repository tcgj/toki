#pragma once

#include "bxdf.hpp"
#include "fresnel.hpp"

namespace TK {
    class Lambert : public BxDF {
    public:
        Lambert(const tkSpectrum &dhr)
            : BxDF(BxDFType(BXDF_REFLECTIVE | BXDF_DIFFUSE)), dhr(dhr) {}

        tkSpectrum operator()(const tkVec3f &wo, const tkVec3f &wi) const;

    private:
        tkSpectrum dhr;  // directional-hemispherical reflectance
    };
} // namespace TK
