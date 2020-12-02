#pragma once

#include "core/integrator.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    enum LightSampleStrategy { POWER };

    class PathTracingIntegrator : public SamplerIntegrator {
    public:
        PathTracingIntegrator(tkInt maxDepth, std::shared_ptr<Camera> camera,
                              std::shared_ptr<Sampler> sampler, LightSampleStrategy strategy = POWER)
            : SamplerIntegrator(camera, sampler), maxDepth(maxDepth), strategy(strategy) {}

        void preprocess(const Scene& scene) override;
        tkSpectrum Li(const Scene& scene, const Ray& r, Sampler& sampler, tkInt depth) const override;

    private:
        tkInt maxDepth;
        LightSampleStrategy strategy;
        std::unique_ptr<Distribution> lightDist;
    };
}  // namespace TK
