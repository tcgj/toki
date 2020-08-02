#include "directional.hpp"

#include "core/scene.hpp"

namespace TK {
    void DirectionalLight::preprocess(const Scene &scene) {
        // Obtain scene radius and scene center from scene data
        scene.worldBoundingBox().boundingSphere(&sceneCenter, &sceneRadius);
    }

    tkSpectrum DirectionalLight::power() const {
        // Overestimate of the power received by the scene
        // using a projected disk perpendicular to light direction
        return radiance * TK_PI * sceneRadius * sceneRadius;
    }

    tkSpectrum DirectionalLight::sample(const Interaction &ref, tkVec3f *wi,
                                        const tkVec2f &samp, tkFloat *pdf,
                                        OcclusionChecker *occCheck) const {
        *wi = -dir;
        *pdf = 1;
        tkPoint3f lightRef = ref.p - 2 * sceneRadius * dir;
        *occCheck = OcclusionChecker(ref, Interaction(lightRef));

        return radiance;
    }
} // namespace TK
