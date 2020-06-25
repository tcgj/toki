#include "perspective.hpp"

#include "core/transform.hpp"

namespace TK {
    PerspectiveCamera::PerspectiveCamera(
        const Transform &cameraToWorld, const tkAABBf screen,
        tkFloat lensRadius, tkFloat focalLength,
        tkFloat fovy /*, const Medium *medium, Image *image*/)
        : ProjectionCamera(cameraToWorld,
                           perspective(fovy, TK_EPSILON, 1000.0f), screen,
                           lensRadius, focalLength) {
        // dx = imageToCamera(tkVec3f(1, 0, 0));
        // dy = imageToCamera(tkVec3f(0, 1, 0));
    }

    tkFloat PerspectiveCamera::generateRay(const CameraSample &sample, Ray *r) const {
        tkPoint3f cs_pos = imageToCamera(tkPoint3f(sample.imgCoord));
        *r = Ray(tkPoint3f(), normalize(tkVec3f(cs_pos)));
        *r = cameraToWorld(*r);
        return 1;
    }
}  // namespace TK
