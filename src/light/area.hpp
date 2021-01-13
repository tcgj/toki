#pragma once

#include "core/light.hpp"
#include "core/shape.hpp"

namespace TK {
    class AreaLight : public Light {
    public:
        AreaLight(const Transform& lightToWorld, const std::shared_ptr<Shape>& shape,
                  const tkSpectrum& radiance)
            : Light(lightToWorld), m_Shape(shape), m_Radiance(radiance), m_Area(shape->surfaceArea()) {}

        bool isDelta() const override;

        tkSpectrum power() const override;

        tkSpectrum Le(const SurfaceInteraction& its, const Vec3f& wo) const override;

        tkSpectrum sample(const Interaction& ref, const Vec2f& u, Vec3f& out_wi, tkFloat& out_pdf,
                          OcclusionChecker& out_checker) const override;

        tkFloat getPdf(const Interaction& ref, const Vec3f& wi) const override;

    private:
        std::shared_ptr<Shape> m_Shape;
        tkSpectrum m_Radiance;
        tkFloat m_Area;
    };
}  // namespace TK
