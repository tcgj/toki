#pragma once

#include "core/interaction.hpp"

namespace TK {
    class SurfaceInteraction : public Interaction {
    public:
        const Shape *shape = nullptr;
        const Primitive *primitive = nullptr;
    };
} // namespace TK