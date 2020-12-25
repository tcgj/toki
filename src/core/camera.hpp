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
            : m_CameraToWorld(cameraToWorld) /*, m_Medium(medium)*/, m_Image(image) {}

        virtual ~Camera() = default;

        virtual tkFloat generateRay(const CameraSample& sample, Ray& r) const = 0;

        Transform m_CameraToWorld;
        Image* m_Image;
        // const Medium* m_Medium;
    };

    class ProjectionCamera : public Camera {
    public:
        ProjectionCamera(const Transform& cameraToWorld, const Transform& cameraToNDC, tkFloat lensRadius,
                         tkFloat focalLength
                         /*, const Medium *medium*/,
                         Image* image)
            : Camera(cameraToWorld /*, medium*/, image), m_LensRadius(lensRadius), m_FocalLength(focalLength) {
            // image space has boundaries from (0, 0) to (res.x, res.y)
            Transform imageToNDC = translate(Vec3f(-1, -1, 0)) * scale((tkFloat)2 / image->m_Resolution.x,
                                                                         (tkFloat)2 / image->m_Resolution.y, 1);
            m_ImageToCamera = inverse(cameraToNDC) * imageToNDC;
        }

        // protected:
        Transform m_ImageToCamera;
        tkFloat m_LensRadius;
        tkFloat m_FocalLength;
    };
}  // namespace TK
