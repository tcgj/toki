#pragma once

#include "system/system.hpp"

namespace TK {
    class Sampler {
    public:
        Sampler() = default;
        virtual ~Sampler() = default;

        virtual void sample1D(tkFloat *samples, tkUInt count) const = 0;
        virtual void sample2D(tkVec2f *samples, tkUInt countX, tkUInt countY) const = 0;
    };
} // namespace TK
