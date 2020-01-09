#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

class Random {
public:
    static void seed(uint_fast32_t val) {
        gen = std::mt19937(val);
    }

    static float nextFloat() {
        return dist(gen);
    }

private:
    Random() {}
    static std::mt19937 gen;
    static std::uniform_real_distribution<float> dist;
};

#endif // RANDOM_HPP