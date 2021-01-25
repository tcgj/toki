#pragma once

#include "core/integrator.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    enum LightSampleStrategy { POWER };

    class PathTracingIntegrator : public Integrator {
    public:
        PathTracingIntegrator(int maxDepth, LightSampleStrategy strategy = POWER)
            : m_MaxDepth(maxDepth), m_Strategy(strategy) {}

        void preprocess(const Scene& scene) override;

        tkSpectrum Li(const Scene& scene, const Ray& r, Sampler& sampler, int depth) const override;

    private:
        int m_MaxDepth;
        LightSampleStrategy m_Strategy;
        std::unique_ptr<Distribution> m_LightDist;
    };
}  // namespace TK
