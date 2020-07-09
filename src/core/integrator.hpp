#pragma once

#include "system/system.hpp"

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

        void render(const Scene &scene) override;
        virtual tkSpectrum computeLi(const Scene &scene, const Ray &r, Sampler &sampler, tkUInt depth = 0) const = 0;

    private:
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Sampler> sampler;
    };
} // namespace TK
