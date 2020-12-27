#include "orthographic.hpp"

#include "core/ray.hpp"

namespace TK {
    OrthographicCamera::OrthographicCamera(const Transform& cameraToWorld, tkFloat lensRadius,
                                           tkFloat focalLength, Image* image)
        : ProjectionCamera(cameraToWorld, orthographic(0, 1), lensRadius, focalLength, image) {}

    Ray OrthographicCamera::generateRay(int x, int y, const CameraSample& sample) const {
        Point3f samplePos(x + sample.pixelOffset.x, y + sample.pixelOffset.y, 0);
        Point3f origin = m_ImageToCamera(samplePos);
        Ray r(origin, Vec3f(0, 0, -1));
        return m_CameraToWorld(r);
    }
}  // namespace TK
