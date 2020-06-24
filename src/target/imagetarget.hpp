#pragma once

#include <string>

#include "system/system.hpp"
#include "math/math.hpp"
#include "geometry/aabb.hpp"

namespace TK {
    class ImageTarget {
    public:
        ImageTarget(const tkVec2i &res, const std::string &filename);

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
