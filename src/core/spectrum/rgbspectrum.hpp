#pragma once

#include "spectrum.hpp"

namespace TK {
    class RGBSpectrum : public Spectrum<3> {
    public:
        RGBSpectrum(tkFloat f = 0.0f) : Spectrum(f) {}
        RGBSpectrum(const Spectrum<3> &s) : Spectrum(s) {}

        tkVec3f toRGB() const;
        static RGBSpectrum fromRGB(const tkVec3f &rgb);
    };
} // namespace TK