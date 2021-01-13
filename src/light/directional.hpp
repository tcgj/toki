#pragma once

#include "core/light.hpp"

namespace TK {
    class DirectionalLight : public Light {
    public:
        DirectionalLight(const Transform& lightToWorld, const tkSpectrum& radiance)
            : Light(lightToWorld), m_Radiance(radiance), m_Dir(normalize(lightToWorld(Vec3f::right))) {}

        void preprocess(const Scene& scene) override;

        tkSpectrum power() const override;

        tkSpectrum sample(const Interaction& ref, const Vec2f& u, Vec3f& out_wi, tkFloat& out_pdf,
                          OcclusionChecker& out_checker) const override;

    private:
        tkSpectrum m_Radiance;
        Vec3f m_Dir;
        Point3f m_SceneCenter;
        tkFloat m_SceneRadius;
    };
}  // namespace TK
