#pragma once

#include <string>

#include "system/toki.hpp"
#include "math/math.hpp"
#include "spectrum.hpp"

namespace TK {
    class Image {
    public:
        Image(const tkVec2i &res, const std::string &filename);

        tkFloat getAspectRatio() const;
        tkVec3f getPixelColor(const tkPoint2i &imgCoord) const;
        void updatePixelColor(const tkPoint2i &imgCoord,
                              const tkSpectrum &colorContribution);
        virtual tkVec3f gammaCorrect(const tkVec3f &rgb) const;
        virtual void write() = 0;

        tkVec2i resolution;
        std::string filename;
    protected:
        std::unique_ptr<tkSpectrum[]> pixels;
    };
} // namespace TK
