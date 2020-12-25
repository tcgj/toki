#include "png.hpp"

#include "system/imageio.hpp"

namespace TK {
    void PNGImage::write() {
        unsigned char imageData[3 * m_Resolution.x * m_Resolution.y];

        int offset = 0;
        for (int y = 0; y < m_Resolution.y; ++y) {
            for (int x = 0; x < m_Resolution.x; ++x) {
                Vec3f color = getPixelColor(Point2i(x, y));
                color = gammaCorrect(color) * (tkFloat)255;

                imageData[offset] = clamp(color.r, 0, 255);
                imageData[offset + 1] = clamp(color.g, 0, 255);
                imageData[offset + 2] = clamp(color.b, 0, 255);

                offset += 3;
            }
        }
        writeToPngFile(m_Filename.c_str(), imageData, m_Resolution.x, m_Resolution.y, 3);
    }
}  // namespace TK
