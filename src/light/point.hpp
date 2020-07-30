#pragma once

#include "core/light.hpp"

namespace TK {
    class PointLight : public Light {
    public:
        PointLight(const Transform &lightToWorld, const tkSpectrum &intensity)
            : Light(lightToWorld), intensity(intensity) {
            // Bypassing transform application, since the light source is defined at (0, 0, 0) in light
            Matrix44 mat = lightToWorld.getMatrix();
            pos = tkPoint3f(mat.data[3], mat.data[7], mat.data[11]);
        }

        tkSpectrum power() const;
        tkSpectrum sample(const Interaction &ref, tkVec3f *wi, tkVec2f &samp,
                          tkFloat *pdf, OcclusionChecker *occCheck) const;

    private:
        tkSpectrum intensity;
        tkPoint3f pos;
    };
} // namespace TK
