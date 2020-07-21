#pragma once

#include "system/toki.hpp"

namespace TK {
    class Material {
    public:
        virtual ~Material() = default;
        virtual void computeScattering(Scattering *scattering) const = 0;
    };
} // namespace TK
