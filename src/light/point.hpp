#pragma once

#include "core/light.hpp"

namespace TK {
    class PointLight : public Light {
    public:
        PointLight(const Transform& lightToWorld, const tkSpectrum& intensity)
            : Light(lightToWorld), intensity(intensity) {
            // Bypassing transform application, since the light source is defined at (0, 0, 0) in light
            Matrix44 mat = lightToWorld.getMatrix();
            pos = Point3f(mat.m_Entries[3], mat.m_Entries[7], mat.m_Entries[11]);
        }

        tkSpectrum power() const;
        tkSpectrum sample(const Interaction& ref, Vec3f* wi, const Vec2f& samp, tkFloat* pdf,
                          OcclusionChecker* occCheck) const;

    private:
        tkSpectrum intensity;
        Point3f pos;
    };
}  // namespace TK
