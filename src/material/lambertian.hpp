#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "material/material.hpp"
#include "sampling/sampling.hpp"

class Lambertian : public Material {
public:
    Lambertian(const Vec3& a) : albedo(a) {}

    bool computeScattering(const Ray& rIn, const RayInteraction& ri, Vec3& attenuation,
                 Ray& scattered) const {
        Point3 target = ri.point + ri.normal + unitSphereUniform();
        scattered = Ray(ri.point, target - ri.point);
        attenuation = albedo;
        return true;
    }

    Vec3 albedo;
};

#endif // LAMBERTIAN_HPP