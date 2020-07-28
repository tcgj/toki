#include "image.hpp"

namespace TK {
    Image::Image(const tkVec2i &res, const std::string &filename)
        : resolution(res), filename(filename) {
        pixels = std::make_unique<tkSpectrum[]>(res.x * res.y);
    }

    tkFloat Image::getAspectRatio() const {
        return (tkFloat)resolution.x / resolution.y;
    }

    tkVec3f Image::getPixelColor(const tkPoint2i &imgCoord) const {
        return pixels[imgCoord.y * resolution.x + imgCoord.x].toRGB();
    }

    void Image::updatePixelColor(const tkPoint2i &imgCoord, const tkSpectrum &colorContribution) {
        pixels[imgCoord.y * resolution.x + imgCoord.x] += colorContribution;
    }

    tkVec3f Image::gammaCorrect(const tkVec3f &rgb) const {
        tkVec3f ret;
        for (tkInt i = 0; i < 3; ++i) {
            if (rgb[i] <= 0.0031308f)
                ret[i] = 12.92f * rgb[i];
            else
                ret[i] = 1.055f * std::pow(rgb[i], 1.0f / 2.4f) - 0.055f;
        }
        return ret;
    }
} // namespace TK
