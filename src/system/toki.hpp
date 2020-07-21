#pragma once

#include <cmath>
#include <iostream>
#include <vector>

//#define TK_DEBUG_MODE

//#define TK_ENABLE_ASSERTS
#include "error.hpp"
//#define TK_USE_DOUBLE
#include "types.hpp"

namespace TK {
    struct Options {
        tkInt threadCount = 0;
        std::string outfile;
    };
} // namespace TK
