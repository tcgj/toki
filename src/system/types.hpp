#pragma once

#include <string>

namespace TK {
    template <typename T> class Vec3;
    template <typename T> class Point3;
    template <typename T> class AABB;
    class Ray;
    class Shape;

    typedef unsigned char tkUChar;
    typedef char tkChar;
    typedef int tkInt;
    typedef float tkFloat;
    typedef float tkF32;
    typedef double tkF64;
    typedef std::string tkString;
    typedef Vec3<tkFloat> tkVec3f;
    typedef Vec3<tkInt> tkVec3i;
    typedef Point3<tkFloat> tkPoint3f;
    typedef Point3<tkInt> tkPoint3i;
    typedef AABB<tkFloat> tkAABBf;
    typedef AABB<tkInt> tkAABBi;
} // namespace TK