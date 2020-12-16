#include "perspective.hpp"

#include "core/transform.hpp"
#include "core/image.hpp"

namespace TK {
    PerspectiveCamera::PerspectiveCamera(const Transform& cameraToWorld, tkFloat lensRadius,
                                         tkFloat focalLength, tkFloat fovy /*, const Medium *medium*/,
                                         Image* image)
        : ProjectionCamera(cameraToWorld, perspective(fovy, image->getAspectRatio(), TK_EPSILON, 1000.0f),
                           lensRadius, focalLength, image) {}

    tkFloat PerspectiveCamera::generateRay(const CameraSample& sample, Ray& r) const {
        Point3f cs_pos = imageToCamera(Point3f(sample.imgCoord));
        r = Ray(Point3f::zero, normalize(Vec3f(cs_pos)));
        r = cameraToWorld(r);
        return 1;
    }
}  // namespace TK
