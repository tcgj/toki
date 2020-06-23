#include "random.hpp"

namespace TK {
    std::uniform_real_distribution<tkFloat> Random::dist_float(0.0f, 1.0f);
    std::mt19937 Random::gen;
} // namespace TK