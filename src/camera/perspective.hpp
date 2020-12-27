#pragma once

#include "core/camera.hpp"

namespace TK {
    class PerspectiveCamera : public ProjectionCamera {
    public:
        PerspectiveCamera(const Transform& cameraToWorld, tkFloat lensRadius, tkFloat focalLength,
                          tkFloat fovy, Image* image);

        Ray generateRay(int x, int y, const CameraSample& sample) const override;
    };
}  // namespace TK
