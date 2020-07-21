#pragma once

#include "core/camera.hpp"

namespace TK {
    class OrthographicCamera : public ProjectionCamera {
    public:
        OrthographicCamera(
            const Transform &cameraToWorld, const tkAABBf &screen,
            tkFloat lensRadius,
            tkFloat focalLength /*, const Medium *medium, Image *image*/)
            : ProjectionCamera(cameraToWorld, orthographic(0, 1), screen, lensRadius, focalLength/*, medium*/, image) {
            // dx = imageToCamera(tkVec3f(1, 0, 0));
            // dy = imageToCamera(tkVec3f(0, 1, 0));
        }

        tkFloat generateRay(const CameraSample &sample, Ray *r) const override;
    private:
        // shift in camera/eye space in the x- and y-axes per pixel
        // tkVec3f dx, dy;
    };
} // namespace TK