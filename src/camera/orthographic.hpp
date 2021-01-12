#pragma once

#include "core/camera.hpp"

namespace TK {
    class OrthographicCamera : public ProjectionCamera {
    public:
        OrthographicCamera(const Transform& cameraToWorld, tkFloat apertureRadius, tkFloat focalLength,
                           Image* image);

        Ray generateRay(int x, int y, const CameraSample& sample) const override;
    };
}  // namespace TK