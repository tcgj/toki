#pragma once

#include "core/sampler.hpp"

namespace TK {
    class StratifiedSampler : public Sampler {
    public:
        StratifiedSampler(tkInt xCount, tkInt yCount, tkInt dimensions);

        void setPixel(const tkPoint2i &pixelCoord) override;
        tkFloat nextFloat() override;
        tkVec2f nextVector() override;
        std::unique_ptr<Sampler> getClone() override;

    private:
        tkInt xCount;
        tkInt yCount;
    };
} // namespace TK
