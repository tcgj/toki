#pragma once

#include "math/math.hpp"

namespace TK {
    struct Interaction {
        Interaction() {}
        Interaction(const tkPoint3f &p, const tkVec3f &n, const tkVec3f &wout)
            : p(p), n(n), wout(wout) {}

        tkPoint3f p;
        tkVec3f n;
        tkVec3f wout;
    };
} // namespace TK