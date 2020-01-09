#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray.hpp"

class Camera {
public:
    Camera(Point3 eye, Point3 at, Vec3 up, float fovy, float aspect,
           float focalLength);
    Ray raycastTo(float s, float t);

private:
    Point3 position;
    Point3 bottomLeft;
    float width;
    float height;
    Vec3 u, v, n;
};

#endif // CAMERA_HPP