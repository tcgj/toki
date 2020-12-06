#pragma once

#include "system/toki.hpp"
#include "bxdf.hpp"

namespace TK {
    class BSDF {
    public:
        BSDF() = default;
        ~BSDF();

        void initialize(const SurfaceInteraction& interaction);
        tkSpectrum evaluate(const Vec3f& worldWo, const Vec3f& worldWi, BxDFType type = BXDF_ALL) const;
        tkSpectrum sample(const Vec3f& worldWo, Vec3f* worldWi, const Vec2f& samp, tkFloat* pdf,
                          BxDFType* sampledType = nullptr, BxDFType type = BXDF_ALL) const;
        tkFloat getPdf(const Vec3f& worldWo, const Vec3f& worldWi, BxDFType type = BXDF_ALL) const;

        bool addContribution(BxDF* bxdf);
        Vec3f worldToLocal(const Vec3f& v) const;
        Vec3f localToWorld(const Vec3f& v) const;

    private:
        static constexpr int MAX_BXDF_COUNT = 8;

        int numBxdf = 0;
        BxDF* bxdfs[MAX_BXDF_COUNT];
        Vec3f n;
        Vec3f t;
        Vec3f bt;
    };
}  // namespace TK
