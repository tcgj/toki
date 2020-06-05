#include "stratified.hpp"

#include "math/math.hpp"
#include "random.hpp"

namespace TK {
    void StratifiedSampler::sample1D(tkFloat *samples, tkUInt count) const {
        tkFloat invCount = (tkFloat)1 / count;
        for (tkUInt i = 0; i < count; ++i) {
            samples[i] = std::min((i + Random::nextFloat()) * invCount, TK_ONE_MINUS_EPSILON);
        }
    }

    void StratifiedSampler::sample2D(tkPoint2f *samples, tkUInt countX, tkUInt countY) const {
        tkFloat invX = (tkFloat)1 / countX;
        tkFloat invY = (tkFloat)1 / countY;
        tkUInt curr = 0;
        for (tkUInt x = 0; x < countX; ++x) {
            for (tkUInt y = 0; y < countY; ++y) {
                samples[curr].x = std::min((x + Random::nextFloat()) * invX,
                                           TK_ONE_MINUS_EPSILON);
                samples[curr].y = std::min((y + Random::nextFloat()) * invY,
                                           TK_ONE_MINUS_EPSILON);
            }
        }
    }
} // namespace TK
