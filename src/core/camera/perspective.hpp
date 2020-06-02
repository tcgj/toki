#pragma once

#include "camera.hpp"

namespace TK {
    class PerspectiveCamera : public ProjectionCamera {
    public:
        PerspectiveCamera(
            const Transform &cameraToWorld, const tkAABBf screen,
            tkFloat lensRadius, tkFloat focalLength,
            tkFloat fovy /*, const Medium *medium, Image *image*/);

        tkFloat generateRay(const CameraSample &sample, Ray *r) const override;
    private:
        // tkVec3f dx, dy;
    };
} // namespace TK
