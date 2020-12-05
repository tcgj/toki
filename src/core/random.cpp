#include "random.hpp"

namespace TK {
    // By specification, it should be inclusive-exclusive.
    // Returns inclusive-inclusive at the moment due to a bug with the standard.
    // Will use [0, 1 - Epsilon] instead of [0, 1) until the bug is fixed
    std::uniform_real_distribution<tkFloat> Random::dist_float(0.0f, TK_ONE_MINUS_EPSILON);
}  // namespace TK