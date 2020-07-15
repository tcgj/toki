#pragma once

#include "core/interaction.hpp"
#include "region/primitive.hpp"

namespace TK {
    class SurfaceInteraction : public Interaction {
    public:
        void updateScatter(Scatter *scatter) {
            if (primitive != nullptr)
                primitive->updateScatter(scatter);
            scattering = scatter;
        }

        const Shape *shape = nullptr;
        const Primitive *primitive = nullptr;
        Scatter *scattering = nullptr;
    };
} // namespace TK