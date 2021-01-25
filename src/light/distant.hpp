#pragma once

#include "core/light.hpp"

namespace TK {
    class DistantLight : public Light {
    public:
        DistantLight(const Transform& lightToWorld, const tkSpectrum& radiance)
            : Light(lightToWorld), m_Radiance(radiance), m_Dir(normalize(lightToWorld(Vec3f::right))) {}

        void preprocess(const Scene& scene) override;

        tkSpectrum power() const override;

        LightSample sample(const Interaction& ref, const Vec2f& u) const override;

    private:
        tkSpectrum m_Radiance;
        Vec3f m_Dir;
        Point3f m_SceneCenter;
        tkFloat m_SceneRadius;
    };
}  // namespace TK
