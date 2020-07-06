#pragma once

#include <string>

#include "system/system.hpp"
#include "math/math.hpp"
#include "aabb.hpp"

namespace TK {
    class Image {
    public:
        Image(const tkVec2i &res, const std::string &filename);

        tkVec3f getPixelColor(const tkPoint2i &imgCoord) const;
        void updatePixelColor(const tkPoint2i &imgCoord,
                              const tkSpectrum &colorContribution);
        void write();

        tkVec2i resolution;
        std::string filename;
    private:
        std::unique_ptr<tkSpectrum[]> pixels;
    };
} // namespace TK
