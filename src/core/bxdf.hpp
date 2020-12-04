#pragma once

#include "system/toki.hpp"

namespace TK {
    enum BxDFType {
        BXDF_REFLECTIVE = 1 << 0,
        BXDF_TRANSMISSIVE = 1 << 1,
        BXDF_DIFFUSE = 1 << 2,
        BXDF_GLOSSY = 1 << 3,
        BXDF_SPECULAR = 1 << 4,
        BXDF_ALL = BXDF_REFLECTIVE | BXDF_TRANSMISSIVE | BXDF_DIFFUSE | BXDF_GLOSSY | BXDF_SPECULAR
    };

    class BxDF {
    public:
        BxDF(BxDFType type) : type(type) {}
        virtual ~BxDF() = default;

        bool matchesType(BxDFType t) const;

        virtual tkSpectrum evaluate(const Vec3f& wo, const Vec3f& wi) const = 0;
        virtual tkSpectrum sample(const Vec3f& wo, Vec3f* wi, const Vec2f& samp, tkFloat* pdf) const;
        virtual tkFloat getPdf(const Vec3f& wo, const Vec3f& wi) const;

        BxDFType type;
    };
}  // namespace TK
