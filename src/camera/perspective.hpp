#pragma once

#include "core/camera.hpp"

namespace TK {
    class PerspectiveCamera : public ProjectionCamera {
    public:
        PerspectiveCamera(const Transform& cameraToWorld, tkFloat apertureRadius, tkFloat focalDistance,
                          tkFloat fovy, Image* image);

        Ray generateRay(int x, int y, const CameraSample& sample) const override;

    private:
        Vec2f m_PixelDelta;
        Vec3f m_ImageDir;
    };
}  // namespace TK
