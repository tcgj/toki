#pragma once

#include "system/system.hpp"

namespace TK {
    enum BxDFType {
        BXDF_REFLECTIVE   = 1 << 0,
        BXDF_TRANSMISSIVE = 1 << 1,
        BXDF_DIFFUSE      = 1 << 2,
        BXDF_GLOSSY       = 1 << 3,
        BXDF_SPECULAR     = 1 << 4,
        BXDF_ALL = BXDF_REFLECTIVE | BXDF_TRANSMISSIVE |
                   BXDF_DIFFUSE | BXDF_GLOSSY | BXDF_SPECULAR
    };

    class BxDF {
    public:
        BxDF(BxDFType type) : type(type) {}
        virtual ~BxDF() = default;

        virtual tkSpectrum operator()(const tkVec3f &wo, const tkVec3f &wi) const = 0;

        bool hasType(BxDFType t) const;
        virtual tkFloat getPdf(const tkVec3f &wo, const tkVec3f &wi) const;
        virtual tkSpectrum sample(const tkVec3f &wo, tkVec3f *wi, const tkVec2f &samp, tkFloat *pdf) const;

        BxDFType type;
    };
} // namespace TK
