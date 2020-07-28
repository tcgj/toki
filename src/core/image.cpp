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
} // namespace TK
