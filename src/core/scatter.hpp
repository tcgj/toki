#pragma once

#include "math/math.hpp"

namespace TK {
    class Scatter {
    public:
        Scatter(const SurfaceInteraction &interaction);
        ~Scatter();

        tkSpectrum operator()(const tkVec3f &wo, const tkVec3f &wi) const;

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
