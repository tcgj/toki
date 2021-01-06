#pragma once

#include "core/integrator.hpp"

namespace TK {
    class AOIntegrator : public Integrator {
    public:
        AOIntegrator(int numSamples, tkFloat maxDist) : m_NumSamples(numSamples), m_MaxDist(maxDist) {}

        tkSpectrum Li(const Scene& scene, const Ray& r, Sampler& sampler, int depth) const override;

    private:
        int m_NumSamples;
        tkFloat m_MaxDist;
    };
} // namespace TK
