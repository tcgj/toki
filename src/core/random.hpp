#pragma once

#include <random>
#include <fstream>

#include "system/toki.hpp"

namespace TK {
    class Random {
    public:
        static void seed(std::uint_least32_t val) {
            gen = std::mt19937(val);
        }

        static tkFloat nextFloat() {
            return dist_float(gen);
        }

        static size_t systemRand(void *dst, size_t dstlen) {
            char *buffer = reinterpret_cast<char *>(dst);
            std::ifstream stream("/dev/urandom",
                                 std::ios_base::binary | std::ios_base::in);
            stream.read(buffer, dstlen);

            return dstlen;
        }

    private:
        Random() = default;
        static std::mt19937 gen;
        static std::uniform_real_distribution<tkFloat> dist_float;
    };
} // namespace TK
