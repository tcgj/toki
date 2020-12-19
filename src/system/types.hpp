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
    struct Mesh;
    class Matrix44;
    class Ray;
    class Transform;
    class Shape;
    class Interaction;
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
    class Integrator;
    class IStream;
    class OStream;
    class Appender;
    class Formatter;
    class Logger;
    class Scheduler;
    class ThreadPool;
    class Task;

#ifdef TK_USE_DOUBLE
    using tkFloat = double;
#else
    using tkFloat = float;
#endif  // TK_USE_DOUBLE
    using Vec2f = Vec2<tkFloat>;
    using Vec2i = Vec2<int>;
    using Vec3f = Vec3<tkFloat>;
    using Vec3i = Vec3<int>;
    using Point2f = Point2<tkFloat>;
    using Point2i = Point2<int>;
    using Point3f = Point3<tkFloat>;
    using Point3i = Point3<int>;
    using AABBf = AABB<tkFloat>;
    using AABBi = AABB<int>;
    using tkSpectrum = RGBSpectrum;
}  // namespace TK