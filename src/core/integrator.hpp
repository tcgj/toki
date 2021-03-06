#pragma once

#include "system/toki.hpp"
#include "light.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    class Integrator {
    public:
        virtual ~Integrator() = default;

        virtual void preprocess(const Scene& scene) {}

        virtual tkSpectrum Li(const Scene& scene, const Ray& r, Sampler& sampler, int depth = 0) const = 0;
    };

    std::shared_ptr<Light> getLightByDist(const Scene& scene, Sampler& sampler, const Distribution& dist,
                                          tkFloat* pdf);

    tkSpectrum miSampleLd(const SurfaceInteraction& ref, const Scene& scene,
                          const std::shared_ptr<Light>& light, Sampler& sampler);
}  // namespace TK
