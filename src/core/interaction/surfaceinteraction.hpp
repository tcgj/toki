#pragma once

#include "interaction.hpp"

namespace TK {
    class SurfaceInteraction : public Interaction {
    public:
        /*BSDF *bsdf = nullptr;*/
        const Shape *shape = nullptr;
        const Primitive *primitive = nullptr;
    };
} // namespace TK