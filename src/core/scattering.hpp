#pragma once

#include "system/toki.hpp"
#include "math/math.hpp"
#include "bxdf.hpp"

namespace TK {
    class Scattering {
    public:
        Scattering(const SurfaceInteraction &interaction);
        ~Scattering();

        tkSpectrum evaluate(const tkVec3f &worldWo, const tkVec3f &worldWi) const;
        tkSpectrum sample(const tkVec3f &worldWo, tkVec3f *worldWi,
                          const tkVec2f &samp, tkFloat *pdf,
                          BxDFType type = BXDF_ALL) const;
        tkFloat getPdf(const tkVec3f &worldWo, const tkVec3f &worldWi,
                       BxDFType type = BXDF_ALL) const;

        bool addContribution(BxDF *bxdf);
        tkVec3f worldToLocal(const tkVec3f &v) const;
        tkVec3f localToWorld(const tkVec3f &v) const;

    private:
        static constexpr tkUInt MAX_BXDF_COUNT = 8;

        tkUInt numBxdf = 0;
        BxDF *bxdfs[MAX_BXDF_COUNT];
        tkVec3f n;
        tkVec3f t;
        tkVec3f bt;
    };
} // namespace TK
