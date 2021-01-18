#pragma once

#include "system/toki.hpp"
#include "spectrum.hpp"

namespace TK {
    enum BxDFType {
        BXDF_REFLECTIVE = 1 << 0,
        BXDF_TRANSMISSIVE = 1 << 1,
        BXDF_DIFFUSE = 1 << 2,
        BXDF_GLOSSY = 1 << 3,
        BXDF_SPECULAR = 1 << 4,
        BXDF_ALL = BXDF_REFLECTIVE | BXDF_TRANSMISSIVE | BXDF_DIFFUSE | BXDF_GLOSSY | BXDF_SPECULAR
    };

    struct BSDFSample {
        BSDFSample() = default;

        BSDFSample(const tkSpectrum& f, const Vec3f& wi, tkFloat pdf) : f(f), wi(wi), pdf(pdf) {}

        explicit operator bool() const {
            return pdf > 0;
        }

        tkSpectrum f;
        Vec3f wi;
        tkFloat pdf = 0;
    };

    class BxDF {
    public:
        BxDF(BxDFType type) : m_Type(type) {}

        virtual ~BxDF() = default;

        bool matchesType(BxDFType t) const;

        virtual tkSpectrum evaluate(const Vec3f& wo, const Vec3f& wi) const = 0;

        virtual BSDFSample sample(const Vec3f& wo, const Vec2f& u, BxDFType type = BXDF_ALL) const;

        virtual tkFloat getPdf(const Vec3f& wo, const Vec3f& wi, BxDFType type = BXDF_ALL) const;

        BxDFType m_Type;
    };
}  // namespace TK
