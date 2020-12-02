#pragma once

#include "system/toki.hpp"

namespace TK {
    namespace Parallel {
        tkUInt getNumCores();
        void initThreads(tkInt threadCount);
        void cleanupThreads();
        void dispatch(tkI64 workSize, tkInt batchSize, std::function<void(tkI64)> func);
        void dispatch2D(const tkVec2i& workSize, tkInt batchSize, std::function<void(tkVec2i)> func);
    }  // namespace Parallel
}  // namespace TK
