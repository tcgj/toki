#pragma once

#include "core/sampler.hpp"

namespace TK {
    class StratifiedSampler : public Sampler {
    public:
        StratifiedSampler(int xCount, int yCount, int dimensions);

        void setPixel(int x, int y) override;

        tkFloat nextFloat() override;

        Vec2f nextVector() override;

        std::unique_ptr<Sampler> getClone() override;

    private:
        int m_XCount;
        int m_YCount;
    };
}  // namespace TK
