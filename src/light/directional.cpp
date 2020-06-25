#include "directional.hpp"

namespace TK {
    void DirectionalLight::preprocess(/*const Scene &scene*/) {
        // Obtain scene radius and scene center from scene data
    }

    bool DirectionalLight::isDelta() const {
        return true;
    }

    tkSpectrum DirectionalLight::power() const {
        // Overestimate of the power received by the scene
        // using a projected disk perpendicular to light direction
        return radiance * TK_PI * sceneRadius * sceneRadius;
    }

    tkSpectrum DirectionalLight::sample(const Interaction &interaction,
                                        tkVec3f *wi, tkFloat *pdf) const {
        *wi = dir;
        *pdf = 1;

        return radiance;
    }
} // namespace TK
