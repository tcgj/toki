#pragma once

#include "core/camera.hpp"

namespace TK {
    class PerspectiveCamera : public ProjectionCamera {
    public:
        PerspectiveCamera(const Transform& cameraToWorld, tkFloat lensRadius, tkFloat focalLength,
                          tkFloat fovy /*, const Medium *medium*/, Image* image);

        tkFloat generateRay(const CameraSample& sample, Ray& r) const override;
    };
}  // namespace TK
