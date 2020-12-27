#include "perspective.hpp"

#include "core/transform.hpp"
#include "core/image.hpp"

namespace TK {
    PerspectiveCamera::PerspectiveCamera(const Transform& cameraToWorld, tkFloat lensRadius,
                                         tkFloat focalLength, tkFloat fovy, Image* image)
        : ProjectionCamera(cameraToWorld, perspective(fovy, image->getAspectRatio(), TK_EPSILON, 1000.0f),
                           lensRadius, focalLength, image) {}

    Ray PerspectiveCamera::generateRay(int x, int y, const CameraSample& sample) const {
        Point3f samplePos(x + sample.pixelOffset.x, y + sample.pixelOffset.y, 0);
        Vec3f dir(m_ImageToCamera(samplePos));
        Ray r(Point3f::zero, normalize(dir));
        return m_CameraToWorld(r);
    }
}  // namespace TK
