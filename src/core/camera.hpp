#pragma once

#include "system/toki.hpp"
#include "transform.hpp"
#include "aabb.hpp"
#include "image.hpp"

namespace TK {
    struct CameraSample {
        tkPoint2f imgCoord;
        tkPoint2f lensCoord;
    };

    class Camera {
    public:
        Camera(const Transform &cameraToWorld /*, const Medium *medium*/, Image *image)
            : cameraToWorld(cameraToWorld)/*, medium(medium)*/, image(image) {}
        virtual ~Camera() = default;

        virtual tkFloat generateRay(const CameraSample &sample, Ray *r) const = 0;

        Transform cameraToWorld;
        Image *image;
        // const Medium *medium;
    };

    class ProjectionCamera : public Camera {
    public:
        ProjectionCamera(const Transform &cameraToWorld,
                         const Transform &cameraToScreen, tkAABBf screen,
                         tkFloat lensRadius, tkFloat focalLength
                         /*, const Medium *medium*/, Image *image)
            : Camera(cameraToWorld/*, medium*/, image),
              cameraToScreen(cameraToScreen),
              lensRadius(lensRadius),
              focalLength(focalLength) {
            // image space has boundaries from (0, 0) to (res.x, res.y)
            // whereas screen space is not scaled to image resolution, and does not bound from origin
            imageToScreen = inverse(
                scale(image->resolution.x / (screen.maxPt.x - screen.minPt.x),
                      image->resolution.y / (screen.maxPt.y - screen.minPt.y), 1) *
                translate(tkVec3f(-screen.minPt.x, -screen.maxPt.y, 0)));
            imageToCamera = inverse(cameraToScreen) * imageToScreen;
        }

    protected:
        Transform imageToCamera, cameraToScreen;
        Transform imageToScreen;
        tkFloat lensRadius, focalLength;
    };
} // namespace TK
