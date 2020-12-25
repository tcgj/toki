#pragma once

#include "system/toki.hpp"
#include "spectrum.hpp"

namespace TK {
    class Image {
    public:
        Image(const Vec2i& res, const std::string& filename);

        virtual ~Image() = default;

        tkFloat getAspectRatio() const;

        Vec3f getPixelColor(const Point2i& pixelCoord) const;

        void updatePixelColor(const Point2i& pixelCoord, const tkSpectrum& colorContribution);

        virtual Vec3f gammaCorrect(const Vec3f& rgb) const;

        virtual void write() = 0;

        Vec2i m_Resolution;
        std::string m_Filename;

    protected:
        std::unique_ptr<tkSpectrum[]> m_Pixels;
        std::unique_ptr<int[]> m_NumSamples;
    };
}  // namespace TK
