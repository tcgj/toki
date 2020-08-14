#pragma once

#include "system/toki.hpp"

namespace TK {
    class Integrator {
    public:
        virtual ~Integrator() = default;

        virtual void preprocess(const Scene &scene) {}
        virtual void render(const Scene &scene) = 0;
    };

    class SamplerIntegrator : public Integrator {
    public:
        SamplerIntegrator(std::shared_ptr<Camera> camera, std::shared_ptr<Sampler> sampler)
            : camera(camera), sampler(sampler) {}

        virtual tkSpectrum Li(const Scene &scene, const Ray &r,
                                     Sampler &sampler, tkInt depth = 0) const = 0;
        void render(const Scene &scene) override;
        tkSpectrum reflectedLi(const SurfaceInteraction &interaction,
                                       const Scene &scene, const Ray &r,
                                       Sampler &sampler, tkInt depth) const;
        tkSpectrum refractedLi(const SurfaceInteraction &interaction,
                                       const Scene &scene, const Ray &r,
                                       Sampler &sampler, tkInt depth) const;

    private:
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Sampler> sampler;
    };
} // namespace TK
