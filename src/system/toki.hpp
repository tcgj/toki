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
#include <unordered_map>
#include <cstdint>

//#define TK_ENABLE_ASSERTS
#include "error.hpp"
#include "types.hpp"

namespace TK {
    int startToki(int argc, char* argv[]);
}  // namespace TK
