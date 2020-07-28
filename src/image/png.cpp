#include "png.hpp"

#include "system/imageio.hpp"

namespace TK {
    tkVec3f PNGImage::gammaCorrect(const tkVec3f &rgb) const {
        tkVec3f ret;
        for (tkInt i = 0; i < 3; ++i) {
            if (rgb[i] <= 0.0031308f)
                ret[i] = 12.92f * rgb[i];
            else
                ret[i] = 1.055f * std::pow(rgb[i], 1.0f / 2.4f) - 0.055f;
        }
        return ret;
    }

    void PNGImage::write() {
        tkUChar imageData[3 * resolution.x * resolution.y];

        tkUInt offset = 0;
        for (tkUInt y = 0; y < resolution.y; ++y) {
            for (tkUInt x = 0; x < resolution.x; ++x) {
                tkVec3f color = getPixelColor(tkPoint2i(x, y));
                color = 255.0f * gammaCorrect(color);

                imageData[offset] = clamp(color.r, 0, 255);
                imageData[offset + 1] = clamp(color.g, 0, 255);
                imageData[offset + 2] = clamp(color.b, 0, 255);

                offset += 3;
            }
        }
        writeToPngFile(filename.c_str(), imageData, resolution.x, resolution.y, 3);
    }
} // namespace TK
