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

//#define TK_ENABLE_ASSERTS
#include "error.hpp"
#include "types.hpp"

namespace TK {
    tkInt startToki(tkInt argc, tkChar *argv[]);
}  // namespace TK
