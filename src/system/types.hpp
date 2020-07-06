#pragma once

#include <string>

namespace TK {
    template <typename T> class Vec2;
    template <typename T> class Vec3;
    template <typename T> class Point2;
    template <typename T> class Point3;
    template <typename T> class AABB;
    struct Matrix44;
    class Ray;
    class Transform;
    struct Mesh;
    class Shape;
    struct Interaction;
    class SurfaceInteraction;
    class Region;
    class Primitive;
    class RGBSpectrum;
    class Light;
    class BxDF;
    class Scatter;
    class Material;
    class Scene;

    typedef unsigned char tkUChar;
    typedef char tkChar;
    typedef unsigned int tkUInt;
    typedef int tkInt;
    typedef float tkFloat;
    typedef float tkF32;
    typedef double tkF64;
    typedef std::string tkString;
    typedef Vec2<tkFloat> tkVec2f;
    typedef Vec2<tkInt> tkVec2i;
    typedef Vec3<tkFloat> tkVec3f;
    typedef Vec3<tkInt> tkVec3i;
    typedef Point2<tkFloat> tkPoint2f;
    typedef Point2<tkInt> tkPoint2i;
    typedef Point3<tkFloat> tkPoint3f;
    typedef Point3<tkInt> tkPoint3i;
    typedef AABB<tkFloat> tkAABBf;
    typedef AABB<tkInt> tkAABBi;
    typedef RGBSpectrum tkSpectrum;
} // namespace TK