#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

namespace Random {
    static std::mt19937 gen;
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    static void seed(uint_fast32_t val) {
        gen = std::mt19937(val);
    }

    static float nextFloat() {
        return dist(gen);
    }

} // namespace Random

#endif // RANDOM_HPP