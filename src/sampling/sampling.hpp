#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#include "random.hpp"
#include "math/math.hpp"

Vec3 unitSphereUniform() {
    Vec3 v;
    do {
        v = Vec3(Random::nextFloat(), Random::nextFloat(),
                        Random::nextFloat()) * 2 - Vec3::one;
    } while (v.squaredMagnitude() >= 1.0f);
    return v;
}

#endif // SAMPLING_HPP