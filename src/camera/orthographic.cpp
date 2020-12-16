#include "orthographic.hpp"

#include "core/ray.hpp"

namespace TK {
    tkFloat OrthographicCamera::generateRay(const CameraSample& sample, Ray& r) const {
        Point3f cs_pos = imageToCamera(Point3f(sample.imgCoord));
        r = Ray(cs_pos, Vec3f(0, 0, -1));  // camera space ray
        // TODO: Handle depth of field here

        // r->medium = medium;
        r = cameraToWorld(r);
        return 1;
    }
}  // namespace TK
