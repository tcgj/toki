#pragma once

#include "core/light.hpp"

namespace TK {
    class PointLight : public Light {
    public:
        PointLight(const Transform& lightToWorld, const tkSpectrum& intensity)
            : Light(lightToWorld), m_Intensity(intensity) {
            // Bypassing transform application, since the light source is defined at (0, 0, 0) in light
            Matrix44 mat = lightToWorld.getMatrix();
            m_Pos = Point3f(mat.m_Entries[3], mat.m_Entries[7], mat.m_Entries[11]);
        }

        tkSpectrum power() const override;

        LightSample sample(const Interaction& ref, const Vec2f& u) const override;

    private:
        tkSpectrum m_Intensity;
        Point3f m_Pos;
    };
}  // namespace TK
