#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "ray.hpp"
#include "primitive/intersectable.hpp"

class Material {
public:
    virtual bool computeScattering(const Ray& rIn, const RayInteraction& ri,
                                   Vec3& attenuation, Ray& scattered) const = 0;
};

#endif // MATERIAL_HPP