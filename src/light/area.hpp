#pragma once

#include "core/light.hpp"
#include "core/shape.hpp"

namespace TK {
    class AreaLight : public Light {
    public:
        AreaLight(const Transform& lightToWorld, const std::shared_ptr<Shape>& shape,
                  const tkSpectrum& radiance)
            : Light(lightToWorld), shape(shape), radiance(radiance), area(shape->surfaceArea()) {}

        bool isDelta() const override;
        tkSpectrum power() const override;
        tkSpectrum Le(const SurfaceInteraction& interaction, const Vec3f& wo) const override;
        tkSpectrum sample(const Interaction& ref, Vec3f* wi, const Vec2f& samp, tkFloat* pdf,
                          OcclusionChecker* occCheck) const override;
        tkFloat getPdf(const Interaction& ref, const Vec3f& wi) const override;

    private:
        std::shared_ptr<Shape> shape;
        tkSpectrum radiance;
        tkFloat area;
    };
}  // namespace TK
