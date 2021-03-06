#pragma once

#include "core/light.hpp"
#include "core/shape.hpp"

namespace TK {
    class AreaLight : public Light {
    public:
        AreaLight(const Transform& lightToWorld, std::shared_ptr<Shape> shape,
                  const tkSpectrum& radiance)
            : Light(lightToWorld), m_Shape(std::move(shape)), m_Radiance(radiance) {
            m_Area = m_Shape->surfaceArea();
        }

        bool isDelta() const override;

        tkSpectrum power() const override;

        tkSpectrum Le(const SurfaceInteraction& its, const Vec3f& wo) const override;

        LightSample sample(const Interaction& ref, const Vec2f& u) const override;

        tkFloat getPdf(const Interaction& ref, const Vec3f& wi) const override;

    private:
        std::shared_ptr<Shape> m_Shape;
        tkSpectrum m_Radiance;
        tkFloat m_Area;
    };
}  // namespace TK
