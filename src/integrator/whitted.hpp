#pragma once

#include "core/integrator.hpp"

namespace TK {
    class WhittedIntegrator : public SamplerIntegrator {
    public:
        WhittedIntegrator(tkInt maxDepth, std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler)
            : SamplerIntegrator(camera, sampler), maxDepth(maxDepth) {}

        tkSpectrum Li(const Scene &scene, const Ray &r, Sampler &sampler,
                             tkInt depth) const override;

    private:
        tkInt maxDepth;
    };
} // namespace TK
