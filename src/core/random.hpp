#pragma once

#include "system/toki.hpp"

namespace TK {
    class Random {
    public:
        static std::uint_least32_t seed() {
            std::uint_least32_t ret;
            systemRand(&ret, sizeof(ret));
            return ret;
        }

        static tkFloat nextFloat() {
            thread_local std::unique_ptr<std::mt19937> gen = std::make_unique<std::mt19937>(seed());

            return dist_float(*gen);
        }

        static size_t systemRand(void* dst, size_t dstlen) {
            char* buffer = reinterpret_cast<char*>(dst);
            std::ifstream stream("/dev/urandom", std::ios_base::binary | std::ios_base::in);
            stream.read(buffer, dstlen);

            return dstlen;
        }

    private:
        Random() = default;
        static std::uniform_real_distribution<tkFloat> dist_float;
    };
}  // namespace TK
