#pragma once

#include "core/interaction.hpp"
#include "region/primitive.hpp"

namespace TK {
    class SurfaceInteraction : public Interaction {
    public:
        void computeScattering(Scattering *s) {
            if (primitive != nullptr)
                primitive->computeScattering(s);
            scattering = s;
        }

        const Shape *shape = nullptr;
        const Primitive *primitive = nullptr;
        Scattering *scattering = nullptr;
    };
} // namespace TK