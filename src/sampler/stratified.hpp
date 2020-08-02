#pragma once

#include "core/sampler.hpp"

namespace TK {
    class StratifiedSampler : public Sampler {
    public:
        StratifiedSampler(tkUInt xCount, tkUInt yCount, tkUInt dimensions);

        void setPixel(const tkPoint2i &pixelCoord) override;
        tkFloat nextFloat() override;
        tkVec2f nextVector() override;
        std::unique_ptr<Sampler> getClone() override;

    private:
        tkUInt xCount, yCount;
    };
} // namespace TK
