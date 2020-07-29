#pragma once

#include "core/light.hpp"
#include "core/spectrum.hpp"

namespace TK {
    class DirectionalLight : public Light {
    public:
        DirectionalLight(const Transform &lightToWorld,
                         const tkSpectrum &radiance)
            : Light(lightToWorld), radiance(radiance),
              dir(normalize(lightToWorld(tkVec3f::right))) {}

        void preprocess(const Scene &scene);
        tkSpectrum power() const;
        tkSpectrum sample(const Interaction &ref, tkVec3f *wi,
                          tkFloat *pdf, OcclusionChecker *occCheck) const;

    private:
        tkSpectrum radiance;
        tkVec3f dir;
        tkPoint3f sceneCenter;
        tkFloat sceneRadius;
    };
} // namespace TK
