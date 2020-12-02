#pragma once

#include "core/camera.hpp"

namespace TK {
    class OrthographicCamera : public ProjectionCamera {
    public:
        OrthographicCamera(const Transform& cameraToWorld, tkFloat lensRadius,
                           tkFloat focalLength /*, const Medium *medium*/, Image* image)
            : ProjectionCamera(cameraToWorld, orthographic(0, 1), lensRadius, focalLength /*, medium*/,
                               image) {}

        tkFloat generateRay(const CameraSample& sample, Ray* r) const override;
    };
}  // namespace TK