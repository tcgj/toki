#include "png.hpp"

#include "system/imageio.hpp"

namespace TK {
    void PNGImage::write() {
        tkUChar imageData[3 * resolution.x * resolution.y];

        tkInt offset = 0;
        for (tkInt y = 0; y < resolution.y; ++y) {
            for (tkInt x = 0; x < resolution.x; ++x) {
                tkVec3f color = getPixelColor(tkPoint2i(x, y));
                color = gammaCorrect(color) * (tkFloat)255;

                imageData[offset] = clamp(color.r, 0, 255);
                imageData[offset + 1] = clamp(color.g, 0, 255);
                imageData[offset + 2] = clamp(color.b, 0, 255);

                offset += 3;
            }
        }
        writeToPngFile(filename.c_str(), imageData, resolution.x, resolution.y, 3);
    }
}  // namespace TK
