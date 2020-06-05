#pragma once

#include <random>

#include "system/system.hpp"

namespace TK {
    class Random {
    public:
        static void seed(uint_fast32_t val);
        static tkFloat nextFloat();

    private:
        Random() = default;
        static std::mt19937 gen;
    };
} // namespace TK
