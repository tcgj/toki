#pragma once

#include "core/light.hpp"

namespace TK {
    class DirectionalLight : public Light {
    public:
        DirectionalLight(const Transform& lightToWorld, const tkSpectrum& radiance)
            : Light(lightToWorld), radiance(radiance), dir(normalize(lightToWorld(Vec3f::right))) {}

        void preprocess(const Scene& scene) override;

        tkSpectrum power() const override;

        tkSpectrum sample(const Interaction& ref, Vec3f* wi, const Vec2f& samp, tkFloat* pdf,
                          OcclusionChecker* occCheck) const override;

    private:
        tkSpectrum radiance;
        Vec3f dir;
        Point3f sceneCenter;
        tkFloat sceneRadius;
    };
}  // namespace TK
