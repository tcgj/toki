#pragma once

#include "system/system.hpp"
#include "geometry/transform.hpp"

namespace TK {
    class Light {
    public:
        Light(const Transform &lightToWorld) : lightToWorld(lightToWorld) {}
        virtual ~Light() = default;

        virtual void preprocess(/*const Scene &scene*/) {}
        virtual bool isDelta() const {
            return true;
        }
        virtual bool isInfinite() const {
            return false;
        }
        virtual tkSpectrum power() const = 0;
        virtual tkSpectrum sample(const Interaction &interaction, tkVec3f *wi, tkFloat *pdf) const = 0;

        Transform lightToWorld;
    };
} // namespace TK
