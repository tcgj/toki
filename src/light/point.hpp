#pragma once

#include "core/light.hpp"
#include "spectrum/rgbspectrum.hpp"

namespace TK {
    class PointLight : public Light {
    public:
        PointLight(const Transform &lightToWorld, const tkSpectrum &intensity)
            : Light(lightToWorld), intensity(intensity) {
            // Bypassing transform application, since the light source is defined at (0, 0, 0) in light
            Matrix44 mat = lightToWorld.getMatrix();
            pos = tkPoint3f(mat.data[3], mat.data[7], mat.data[11]);
        }

        bool isDelta() const;
        tkSpectrum power() const;
        tkSpectrum sample(const Interaction &interaction, tkVec3f *wi, tkFloat *pdf) const;

    private:
        tkSpectrum intensity;
        tkPoint3f pos;
    };
} // namespace TK
