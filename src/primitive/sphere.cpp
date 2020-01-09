#include "sphere.hpp"

bool Sphere::intersects(const Ray& r, float tMin, float tMax, RayInteraction& ri) const {
    Vec3 c_o = r.origin - center;
    float a = dot(r.direction, r.direction);
    float b = dot(r.direction, c_o); // Removed redundant factor of 2
    float c = dot(c_o, c_o) - radius * radius;
    float discriminant = b * b - a * c;

    float t;
    if (discriminant == 0) {
        t = -b / a;
    } else if (discriminant > 0) {
        float tn = (-b - sqrtf(discriminant)) / a;
        float tp = (-b + sqrtf(discriminant)) / a;
        t = tn < tMin ? tp : tn;
    } else {
        return false;
    }

    if (t < tMin || t > tMax) {
        return false;
    }

    ri.t = t;
    ri.point = r(t);
    ri.normal = (ri.point - center) / radius;
    return true;
}