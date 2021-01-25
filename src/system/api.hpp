#pragma once

#include "toki.hpp"
#include "thread.hpp"
#include "logger.hpp"

namespace TK {
    enum InputFileType {
        TOKI = 0,
        OBJ = 1
    };

    struct Options {
        std::string outfile;
        int threadCount = -1;
        bool fastRender = false;
        bool gui = false;
        int inputType = TOKI;
    };

    namespace RenderAPI {
        void configure(const Options& options);

        void parse(const std::string& inputFile);

        void render();

        void shutdown();
    }  // namespace RenderAPI

    // Serializing API
}  // namespace TK
