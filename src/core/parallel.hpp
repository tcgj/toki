#pragma once

#include "system/toki.hpp"

namespace TK {
    tkUInt getNumCores();
    void initThreads(tkInt threadCount);
    void cleanupThreads();
    void parallelFor(tkI64 loopCount, tkInt batchSize,
                     std::function<void(tkI64)> func);
    void parallelFor2D(const tkVec2i &loopCount, tkInt batchSize,
                       std::function<void(tkVec2i)> func);
}  // namespace TK
