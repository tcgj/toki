#include "perspective.hpp"

#include "core/transform.hpp"
#include "core/image.hpp"
#include "util/samplingutil.hpp"

namespace TK {
    PerspectiveCamera::PerspectiveCamera(const Transform& cameraToWorld, tkFloat apertureRadius,
                                         tkFloat focalDistance, tkFloat fovy, Image* image)
        : ProjectionCamera(cameraToWorld, apertureRadius, focalDistance, image) {
        tkFloat fovScale = tan(degToRad(fovy / 2));
        Vec2f bottomLeft(-image->getAspectRatio() * fovScale, -fovScale);
        m_PixelDelta = (-bottomLeft * 2.0f) / Vec2f(image->m_Resolution);  // top right = - bottom left
        m_ImageDir = Vec3f(bottomLeft, -1);
    }

    Ray PerspectiveCamera::generateRay(int x, int y, const CameraSample& sample) const {
        Vec2f imgOffset(x + sample.pixelRand.x, y + sample.pixelRand.y);
        Vec3f dir = m_ImageDir + Vec3f(m_PixelDelta * imgOffset, 0);
        Ray ray(Point3f::zero, normalize(dir));

        if (m_ApertureRadius > 0) {
            Vec2f sampleAperture = concentricDiskSample(sample.lensRand.x, sample.lensRand.y) * m_ApertureRadius;

            tkFloat tFocal = m_FocalDistance / -ray.d.z;
            Point3f pFocal = ray(tFocal);
            ray.o = Point3f(sampleAperture.x, sampleAperture.y, 0);
            ray.d = normalize(pFocal - ray.o);
        }

        return m_CameraToWorld(ray);
    }
}  // namespace TK
