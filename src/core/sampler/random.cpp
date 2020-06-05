#include "random.hpp"

namespace TK {
    std::uniform_real_distribution<tkFloat> dist_float(0.0f, 1.0f);

    void Random::seed(uint_fast32_t val) {
        gen = std::mt19937(val);
    }

    tkFloat Random::nextFloat() {
        return dist_float(gen);
    }
} // namespace TK