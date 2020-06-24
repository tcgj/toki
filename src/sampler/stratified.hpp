#pragma once

#include "core/sampler.hpp"

namespace TK {
    class StratifiedSampler : public Sampler {
    public:
        StratifiedSampler() = default;

        void sample1D(tkFloat *samples, tkUInt count) const override;
        void sample2D(tkVec2f *samples, tkUInt countX, tkUInt countY) const override;
    };
} // namespace TK
