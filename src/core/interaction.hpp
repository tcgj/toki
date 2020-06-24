#pragma once

#include "system/system.hpp"

namespace TK {
    struct Interaction {
        Interaction() = default;
        Interaction(const tkPoint3f &p, const tkVec3f &n, const tkVec3f &t, const tkVec3f &wo)
            : p(p), n(n), t(t), wo(wo) {}
        virtual ~Interaction() = default;

        tkPoint3f p;
        tkVec3f n;
        tkVec3f t;
        tkVec3f wo;
    };
} // namespace TK