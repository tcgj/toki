#pragma once

#include "petershirley/material/material.hpp"
#include "petershirley/sampling/sampling.hpp"

class Metal : public Material {
public:
    Metal(const tkVec3f& a, float f) : albedo(a) { fuzz = f < 1 ? f : 1; }

    bool computeScattering(const Ray& rIn, const RayInteraction& ri,
                           tkVec3f& attenuation, Ray& scattered) const {
        Vec3 reflectDir = reflect(unitVectorOf(rIn.d), ri.normal);
        scattered = Ray(ri.point, reflectDir + fuzz * unitSphereUniform());
        attenuation = albedo;
        return dot(reflectDir, ri.normal) > 0;
    }

    tkVec3f albedo;
    float fuzz;
};
