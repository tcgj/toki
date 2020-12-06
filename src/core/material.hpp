#pragma once

#include "system/toki.hpp"

namespace TK {
    class Material {
    public:
        virtual ~Material() = default;
        virtual void computeScattering(BSDF* bsdf) const = 0;
    };
}  // namespace TK
