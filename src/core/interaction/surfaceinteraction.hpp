#pragma once

#include "interaction.hpp"
#include "system/types.hpp"

namespace TK {
    class SurfaceInteraction : public Interaction {
    public:
        const Shape *shape = nullptr;
    };
} // namespace TK