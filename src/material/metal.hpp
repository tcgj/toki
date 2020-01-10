#ifndef METAL_HPP
#define METAL_HPP

#include "material/material.hpp"
#include "sampling/sampling.hpp"

class Metal : public Material {
public:
    Metal(const Vec3& a, float f) : albedo(a) { fuzz = f < 1 ? f : 1; }

    bool computeScattering(const Ray& rIn, const RayInteraction& ri,
                           Vec3& attenuation, Ray& scattered) const {
        Vec3 reflectDir = reflect(unitVectorOf(rIn.direction), ri.normal);
        scattered = Ray(ri.point, reflectDir + fuzz * unitSphereUniform());
        attenuation = albedo;
        return dot(reflectDir, ri.normal) > 0;
    }

    Vec3 albedo;
    float fuzz;
};

#endif  // METAL_HPP