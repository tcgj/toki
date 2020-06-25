#pragma once

#include "core/light.hpp"
#include "spectrum/rgbspectrum.hpp"

namespace TK {
    class DirectionalLight : public Light {
    public:
        DirectionalLight(const Transform &lightToWorld,
                         const tkSpectrum &radiance)
            : Light(lightToWorld), radiance(radiance),
              dir(normalize(lightToWorld(tkVec3f::right))) {}

        void preprocess(/*const Scene &scene*/);
        bool isDelta() const;
        tkSpectrum power() const;
        tkSpectrum sample(const Interaction &interaction, tkVec3f *wi,
                          tkFloat *pdf) const;

    private:
        tkSpectrum radiance;
        tkVec3f dir;
        tkPoint3f sceneCenter;
        tkFloat sceneRadius;
    };
} // namespace TK
