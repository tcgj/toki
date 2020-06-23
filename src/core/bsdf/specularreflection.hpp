#pragma once

#include "bxdf.hpp"
#include "fresnel.hpp"

namespace TK {
    class SpecularReflection : public BxDF {
    public:
        SpecularReflection(const tkSpectrum &dhr, Fresnel *fresnel)
            : BxDF(BxDFType(BXDF_REFLECTIVE | BXDF_SPECULAR)),
              dhr(dhr),
              fresnel(fresnel) {}

        tkSpectrum operator()(const tkVec3f &wo, const tkVec3f &wi) const;

        tkSpectrum getSample(const tkVec3f &wo, tkVec3f *wi,
                             tkFloat *pdf) const;

    private:
        tkSpectrum dhr; // directional-hemispherical reflectance
        Fresnel *fresnel = nullptr;
    };
} // namespace TK