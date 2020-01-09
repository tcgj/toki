#include <cmath>

#include "camera.hpp"

Camera::Camera(Point3 eye, Point3 at, Vec3 up, float fovy, float aspect, float focalLength) {
    position = eye;
    n = unitVectorOf(eye - at);
    u = unitVectorOf(cross(up, n));
    v = cross(n, u);

    float theta = M_PI * (fovy * 0.5f) / 180.0f;
    float hh = tanf(theta) * focalLength;
    float hw = hh * aspect;

    bottomLeft = (position - n * focalLength) - u * hw - v * hh;
    width = hw * 2;
    height = hh * 2;
}

Ray Camera::raycastTo(float s, float t) {
    return Ray(position, (bottomLeft + s * u * width + t * v * height) - position);
}