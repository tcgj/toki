#pragma once

#include "system/toki.hpp"
#include "transform.hpp"
#include "aabb.hpp"
#include "image.hpp"

namespace TK {
    struct CameraSample {
        Vec2f pixelRand;
        Vec2f lensRand;
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
        ProjectionCamera(const Transform& cameraToWorld, tkFloat apertureRadius,
                         tkFloat focalDistance, Image* image)
            : Camera(cameraToWorld, image), m_ApertureRadius(apertureRadius), m_FocalDistance(focalDistance) {}

    protected:
        tkFloat m_ApertureRadius;
        tkFloat m_FocalDistance;
    };
}  // namespace TK
