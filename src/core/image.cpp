#include "image.hpp"

#include "system/imageio.hpp"
#include "spectrum.hpp"

namespace TK {
    Image::Image(const tkVec2i &res, const std::string &filename)
        : resolution(res), filename(filename) {
        pixels = std::make_unique<tkSpectrum[]>(res.x * res.y);
    }

    tkVec3f Image::getPixelColor(const tkPoint2i &imgCoord) const {
        return pixels[imgCoord.y * resolution.x + imgCoord.x].toRGB();
    }

    void Image::updatePixelColor(const tkPoint2i &imgCoord, const tkSpectrum &colorContribution) {
        pixels[imgCoord.y * resolution.x + imgCoord.x] += colorContribution;
    }

    void Image::write() {
        tkUChar imageData[3 * resolution.x * resolution.y];

        tkUInt offset = 0;
        for (tkUInt y = 0; y < resolution.y; ++y) {
            for (tkUInt x = 0; x < resolution.x; ++x) {
                tkVec3f color = getPixelColor(tkPoint2i(x, y));
                imageData[offset] = color.r * 255;
                imageData[offset + 1] = color.g * 255;
                imageData[offset + 2] = color.b * 255;

                offset += 3;
            }
        }
        writeToPngFile(filename.c_str(), imageData, resolution.x, resolution.y, 3);
    }
} // namespace TK
