#include "image.hpp"

namespace TK {
    Image::Image(const Vec2i& res, const std::string& filename) : m_Resolution(res), m_Filename(filename) {
        m_Pixels = std::make_unique<tkSpectrum[]>(res.x * res.y);
        m_NumSamples = std::make_unique<int[]>(res.x * res.y);
    }

    tkFloat Image::getAspectRatio() const {
        return (tkFloat)m_Resolution.x / m_Resolution.y;
    }

    Vec3f Image::getPixelColor(const Point2i& pixelCoord) const {
        tkSpectrum avg = m_Pixels[pixelCoord.y * m_Resolution.x + pixelCoord.x] /
                         m_NumSamples[pixelCoord.y * m_Resolution.x + pixelCoord.x];
        return avg.toRGB();
    }

    void Image::updatePixelColor(const Point2i& pixelCoord, const tkSpectrum& colorContribution) {
        m_Pixels[pixelCoord.y * m_Resolution.x + pixelCoord.x] += colorContribution;
        m_NumSamples[pixelCoord.y * m_Resolution.x + pixelCoord.x]++;
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
