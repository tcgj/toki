#pragma once

#include <functional>

#include "system/system.hpp"

namespace TK {
    tkUInt getNumCores();
    void initThreads();
    void cleanupThreads();
    void parallelFor(tkI64 loopCount, tkInt batchSize,
                     std::function<void(tkI64)> func);
    void parallelFor2D(const tkVec2i &loopCount,
                       std::function<void(tkVec2i)> func);
}  // namespace TK
