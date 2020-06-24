#include "image.hpp"

#include "system/io/imageio.hpp"
#include "spectrum/rgbspectrum.hpp"

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
                imageData[offset] = static_cast<tkUChar>(color.r);
                imageData[offset + 1] = static_cast<tkUChar>(color.g);
                imageData[offset + 2] = static_cast<tkUChar>(color.b);

                offset += 3;
            }
        }
        writeToPngFile(filename.c_str(), imageData, resolution.x, resolution.y, 3);
    }
} // namespace TK
