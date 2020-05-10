#ifndef SAMPLING_HPP
#define SAMPLING_HPP

#include "random.hpp"

tkVec3f unitSphereUniform() {
    tkVec3f v;
    do {
        v = tkVec3f(Random::nextFloat(), Random::nextFloat(),
                        Random::nextFloat()) * 2.0f - tkVec3f::one;
    } while (v.squaredMagnitude() >= 1.0f);
    return v;
}

#endif // SAMPLING_HPP