#pragma once

#include "core/sampler.hpp"

namespace TK {
    class StratifiedSampler : public Sampler {
    public:
        StratifiedSampler(int xCount, int yCount, int dimensions);

        void setPixel(const Point2i& pixelCoord) override;

        tkFloat nextFloat() override;

        Vec2f nextVector() override;

        std::unique_ptr<Sampler> getClone() override;

    private:
        int xCount;
        int yCount;
    };
}  // namespace TK
