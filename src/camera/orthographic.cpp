#include "orthographic.hpp"

#include "core/ray.hpp"

namespace TK {
    OrthographicCamera::OrthographicCamera(const Transform& cameraToWorld, tkFloat apertureRadius,
                                           tkFloat focalDistance, Image* image)
        : ProjectionCamera(cameraToWorld, apertureRadius, focalDistance, image) {}

    Ray OrthographicCamera::generateRay(int x, int y, const CameraSample& sample) const {
        // TODO: Fix ortho camera
        Point3f samplePos(x + sample.pixelRand.x, y + sample.pixelRand.y, 0);
        Point3f origin = (samplePos);
        Ray r(origin, Vec3f(0, 0, -1));
        return m_CameraToWorld(r);
    }
}  // namespace TK
