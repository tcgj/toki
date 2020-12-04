#pragma once

#include "system/toki.hpp"

namespace TK {
    namespace Parallel {
        unsigned int getNumCores();
        void initThreads(int threadCount);
        void cleanupThreads();
        void dispatch(int64_t workSize, int batchSize, std::function<void(int64_t)> func);
        void dispatch2D(const Vec2i& workSize, int batchSize, std::function<void(Vec2i)> func);
    }  // namespace Parallel
}  // namespace TK
