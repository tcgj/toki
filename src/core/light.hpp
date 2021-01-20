#pragma once

#include "system/toki.hpp"
#include "transform.hpp"
#include "scene.hpp"
#include "interaction.hpp"
#include "spectrum.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    struct LightSample {
        LightSample() = default;

        LightSample(const tkSpectrum& l, const Vec3f& wi, const Interaction& its, tkFloat pdf)
            : l(l), wi(wi), its(its), pdf(pdf) {}

        bool isOccluded(const Scene& scene, const Interaction& ref) const;

        explicit operator bool() const {
            return pdf > 0;
        }

        tkSpectrum l;
        Vec3f wi;
        Interaction its;
        tkFloat pdf = 0;
    };

    class Light {
    public:
        Light(const Transform& lightToWorld) : m_LightToWorld(lightToWorld) {}

        virtual ~Light() = default;

        virtual void preprocess(const Scene& scene) {}

        virtual bool isDelta() const {
            // Whether the light source is described by a delta distribution,
            // positional wise or directional-wise
            return true;
        }

        virtual tkSpectrum power() const = 0;

        virtual tkSpectrum Le(const SurfaceInteraction& its, const Vec3f& wo) const {
            return 0;
        };

        virtual LightSample sample(const Interaction& ref, const Vec2f& u) const = 0;

        virtual tkFloat getPdf(const Interaction& ref, const Vec3f& wi) const {
            return 0;
        }

    protected:
        Transform m_LightToWorld;
    };

    std::unique_ptr<Distribution> lightPowerDistribution(const Scene& scene);
}  // namespace TK
