#pragma once

#include "system/toki.hpp"
#include "transform.hpp"
#include "aabb.hpp"
#include "image.hpp"

namespace TK {
    struct CameraSample {
        Vec2f pixelOffset;
        Vec2f lensOffset;
    };

    class Camera {
    public:
        Camera(const Transform& cameraToWorld, Image* image)
            : m_CameraToWorld(cameraToWorld), m_Image(image) {}

        virtual ~Camera() = default;

        virtual Ray generateRay(int x, int y, const CameraSample& sample) const = 0;

        Transform m_CameraToWorld;
        Image* m_Image;
    };

    class ProjectionCamera : public Camera {
    public:
        ProjectionCamera(const Transform& cameraToWorld, const Transform& cameraToNDC, tkFloat lensRadius,
                         tkFloat focalLength, Image* image)
            : Camera(cameraToWorld, image), m_LensRadius(lensRadius), m_FocalLength(focalLength) {
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
