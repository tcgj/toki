#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "ray/ray.hpp"

class Camera {
public:
    Camera(tkPoint3f eye, tkPoint3f at, tkVec3f up, float fovy, float aspect,
           float focalLength);
    Ray raycastTo(float s, float t);

private:
    tkPoint3f position;
    tkPoint3f bottomLeft;
    float width;
    float height;
    tkVec3f u, v, n;
};

#endif // CAMERA_HPP