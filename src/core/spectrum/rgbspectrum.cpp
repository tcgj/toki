#include "rgbspectrum.hpp"

namespace TK {
    inline tkVec3f RGBSpectrum::toRGB() const {
        return tkVec3f(c[0], c[1], c[2]);
    }
    inline RGBSpectrum RGBSpectrum::fromRGB(const tkVec3f &rgb) {
        RGBSpectrum ret;
        ret.c[0] = rgb.r;
        ret.c[1] = rgb.g;
        ret.c[2] = rgb.b;
        return ret;
    }
}  // namespace TK
