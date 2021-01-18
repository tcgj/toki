#pragma once

#include "system/toki.hpp"

namespace TK {
    class Material {
    public:
        virtual ~Material() = default;

        virtual BSDF getBSDF(const SurfaceInteraction& its) const = 0;
    };
}  // namespace TK
