#pragma once

#include <iostream>
#include <functional>
#include <thread>
#include <mutex>
#include <fstream>

#include <random>
#include <cmath>
#include <string>
#include <vector>
#include <cstdint>

//#define TK_DEBUG_MODE

//#define TK_ENABLE_ASSERTS
#include "error.hpp"
//#define TK_USE_DOUBLE
#include "types.hpp"

namespace TK {
    tkInt startToki(tkInt argc, tkChar *argv[]);
}  // namespace TK
