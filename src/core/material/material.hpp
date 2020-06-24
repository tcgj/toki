#pragma once

#include "system/system.hpp"

namespace TK {
    class Material {
    public:
        virtual void updateScatter(Scatter *scatter) const = 0;
    };
} // namespace TK
