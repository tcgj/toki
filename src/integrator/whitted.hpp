#pragma once

#include "core/integrator.hpp"

namespace TK {
    class WhittedIntegrator : public Integrator {
    public:
        WhittedIntegrator(int maxDepth)
            : maxDepth(maxDepth) {}

        tkSpectrum Li(const Scene& scene, const Ray& r, Sampler& sampler, int depth) const override;

    private:
        int maxDepth;
    };
}  // namespace TK
