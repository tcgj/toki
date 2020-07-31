#include "perspective.hpp"

#include "core/transform.hpp"
#include "core/image.hpp"

namespace TK {
    PerspectiveCamera::PerspectiveCamera(
        const Transform &cameraToWorld, tkFloat lensRadius, tkFloat focalLength,
        tkFloat fovy/*, const Medium *medium*/, Image *image)
        : ProjectionCamera(cameraToWorld,
                           perspective(fovy, image->getAspectRatio(), TK_EPSILON, 1000.0f),
                           lensRadius, focalLength, image) {}

    tkFloat PerspectiveCamera::generateRay(const CameraSample &sample, Ray *r) const {
        tkPoint3f cs_pos = imageToCamera(tkPoint3f(sample.imgCoord));
        *r = Ray(tkPoint3f::zero, normalize(tkVec3f(cs_pos)));
        *r = cameraToWorld(*r);
        return 1;
    }
}  // namespace TK
