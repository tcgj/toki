#pragma once

#include "system/toki.hpp"
#include "transform.hpp"
#include "aabb.hpp"
#include "image.hpp"

namespace TK {
    struct CameraSample {
        Point2f imgCoord;
        Vec2f lens;
    };

    class Camera {
    public:
        Camera(const Transform& cameraToWorld /*, const Medium *medium*/, Image* image)
            : cameraToWorld(cameraToWorld) /*, medium(medium)*/, image(image) {}

        virtual ~Camera() = default;

        virtual tkFloat generateRay(const CameraSample& sample, Ray* r) const = 0;

        Transform cameraToWorld;
        Image* image;
        // const Medium *medium;
    };

    class ProjectionCamera : public Camera {
    public:
        ProjectionCamera(const Transform& cameraToWorld, const Transform& cameraToNDC, tkFloat lensRadius,
                         tkFloat focalLength
                         /*, const Medium *medium*/,
                         Image* image)
            : Camera(cameraToWorld /*, medium*/, image), lensRadius(lensRadius), focalLength(focalLength) {
            // image space has boundaries from (0, 0) to (res.x, res.y)
            Transform imageToNDC = translate(Vec3f(-1, -1, 0)) * scale((tkFloat)2 / image->resolution.x,
                                                                         (tkFloat)2 / image->resolution.y, 1);
            imageToCamera = inverse(cameraToNDC) * imageToNDC;
        }

        // protected:
        Transform imageToCamera;
        tkFloat lensRadius;
        tkFloat focalLength;
    };
}  // namespace TK
