#include "random.hpp"

void Random::seed(uint_fast32_t val) {
    gen = std::mt19937(val);
}

float Random::nextFloat() {
    return dist(gen);
}

std::mt19937 Random::gen;
std::uniform_real_distribution<float> Random::dist(0.0f, 1.0f);