#pragma once

#include "system/system.hpp"

namespace TK {
    struct Interaction {
        Interaction() = default;
        Interaction(const tkPoint3f &p, const tkVec3f &n, const tkVec3f &wout)
            : p(p), n(n), wout(wout) {}
        virtual ~Interaction() = default;

        tkPoint3f p;
        tkVec3f n;
        tkVec3f wout;
    };
} // namespace TK