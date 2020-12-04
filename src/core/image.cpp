#include "image.hpp"

namespace TK {
    Image::Image(const Vec2i& res, const std::string& filename) : resolution(res), filename(filename) {
        pixels = std::make_unique<tkSpectrum[]>(res.x * res.y);
    }

    tkFloat Image::getAspectRatio() const {
        return (tkFloat)resolution.x / resolution.y;
    }

    Vec3f Image::getPixelColor(const Point2i& pixelCoord) const {
        return pixels[pixelCoord.y * resolution.x + pixelCoord.x].toRGB();
    }

    void Image::updatePixelColor(const Point2i& pixelCoord, const tkSpectrum& colorContribution) {
        pixels[pixelCoord.y * resolution.x + pixelCoord.x] += colorContribution;
    }

    Vec3f Image::gammaCorrect(const Vec3f& rgb) const {
        Vec3f ret;
        for (int i = 0; i < 3; ++i) {
            if (rgb[i] <= 0.0031308f)
                ret[i] = 12.92f * rgb[i];
            else
                ret[i] = 1.055f * std::pow(rgb[i], 1.0f / 2.4f) - 0.055f;
        }
        return ret;
    }
}  // namespace TK
