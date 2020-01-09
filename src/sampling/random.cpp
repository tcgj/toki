#include "random.hpp"

std::mt19937 Random::gen;
std::uniform_real_distribution<float> Random::dist(0.0f, 1.0f);