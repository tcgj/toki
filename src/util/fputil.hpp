#pragma once

#include "system/toki.hpp"

namespace TK {
    float intToFloat(int32_t i) {
        float f;
        memcpy(&f, &i, sizeof(int32_t));
        return f;
    }

    int32_t floatToInt(float f) {
        int32_t i;
        memcpy(&i, &f, sizeof(float));
        return i;
    }
} // namespace TK
