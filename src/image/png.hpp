#pragma once

#include "core/image.hpp"

namespace TK {
    class PNGImage : public Image {
    public:
        PNGImage(const Vec2i& res, const std::string& filename) : Image(res, filename) {}

        void write() override;
    };
}  // namespace TK
