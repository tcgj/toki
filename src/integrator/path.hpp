#pragma once

#include "core/integrator.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    enum LightSampleStrategy { POWER };

    class PathTracingIntegrator : public Integrator {
    public:
        PathTracingIntegrator(int maxDepth, LightSampleStrategy strategy = POWER)
            : maxDepth(maxDepth), strategy(strategy) {}

        void preprocess(const Scene& scene) override;

        tkSpectrum Li(const Scene& scene, const Ray& r, Sampler& sampler, int depth) const override;

    private:
        int maxDepth;
        LightSampleStrategy strategy;
        std::unique_ptr<Distribution> lightDist;
    };
}  // namespace TK
