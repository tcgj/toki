#pragma once

#include "system/system.hpp"
#include "core/transform.hpp"

namespace TK {
    class Light {
    public:
        Light(const Transform &lightToWorld) : lightToWorld(lightToWorld) {}
        virtual ~Light() = default;

        virtual void preprocess(/*const Scene &scene*/) {}
        virtual bool isDelta() const {
            // Whether the light source is described by a delta distribution,
            // positional wise or directional-wise
            return false;
        }
        virtual tkSpectrum power() const = 0;
        virtual tkSpectrum sample(const Interaction &interaction, tkVec3f *wi, tkFloat *pdf) const = 0;

    protected:
        Transform lightToWorld;
    };
} // namespace TK
