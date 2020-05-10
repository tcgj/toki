#pragma once

#include "petershirley/primitive/intersectable.hpp"
#include "ray/ray.hpp"

class Material {
public:
    virtual bool computeScattering(const Ray& rIn, const RayInteraction& ri,
                                   Vec3& attenuation, Ray& scattered) const = 0;
};
