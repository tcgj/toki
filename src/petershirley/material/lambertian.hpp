#pragma once

#include "petershirley/material/material.hpp"
#include "petershirley/sampling/sampling.hpp"

class Lambertian : public Material {
public:
    Lambertian(const tkVec3f& a) : albedo(a) {}

    bool computeScattering(const Ray& rIn, const RayInteraction& ri, tkVec3f& attenuation,
                 Ray& scattered) const {
        tkPoint3f target = ri.point + ri.normal + unitSphereUniform();
        scattered = Ray(ri.point, target - ri.point);
        attenuation = albedo;
        return true;
    }

    tkVec3f albedo;
};