#pragma once

#include <random>

#include "system/toki.hpp"

namespace TK {
    class Random {
    public:
        static void seed(uint_fast32_t val) {
            gen = std::mt19937(val);
        }

        static tkFloat nextFloat() {
            return dist_float(gen);
        }

    private:
        Random() = default;
        static std::mt19937 gen;
        static std::uniform_real_distribution<tkFloat> dist_float;
    };
} // namespace TK
