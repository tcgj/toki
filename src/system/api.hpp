#pragma once

#include "toki.hpp"
#include "thread.hpp"
#include "logger.hpp"

namespace TK {
    struct Options {
        std::string outfile;
        int threadCount = -1;
        bool fastRender = false;
        bool gui = false;
    };

    namespace RenderAPI {
        void configure(const Options& options);

        void parse(std::string inputFile);

        void render();

        void shutdown();
    }  // namespace RenderAPI

    // Serializing API
}  // namespace TK
