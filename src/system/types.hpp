#pragma once

namespace TK {
    template <typename T>
    class Vec2;
    template <typename T>
    class Vec3;
    template <typename T>
    class Point2;
    template <typename T>
    class Point3;
    template <typename T>
    class AABB;
    struct Matrix44;
    struct Mesh;
    struct Interaction;
    class Ray;
    class Transform;
    class Shape;
    class SurfaceInteraction;
    class Region;
    class Primitive;
    class RGBSpectrum;
    class Light;
    class BxDF;
    class BSDF;
    class Material;
    class Camera;
    class Scene;
    class Sampler;
    class IStream;
    class OStream;

#ifdef TK_USE_DOUBLE
    typedef double tkFloat;
#else
    typedef float tkFloat;
#endif  // TK_USE_DOUBLE
    typedef Vec2<tkFloat> Vec2f;
    typedef Vec2<int> Vec2i;
    typedef Vec3<tkFloat> Vec3f;
    typedef Vec3<int> Vec3i;
    typedef Point2<tkFloat> Point2f;
    typedef Point2<int> Point2i;
    typedef Point3<tkFloat> Point3f;
    typedef Point3<int> Point3i;
    typedef AABB<tkFloat> AABBf;
    typedef AABB<int> AABBi;
    typedef RGBSpectrum tkSpectrum;
}  // namespace TK