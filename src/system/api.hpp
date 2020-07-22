#pragma once

#include "system/toki.hpp"
#include "core/parallel.hpp"

namespace TK {
    struct Options {
        tkInt threadCount = 0;
        std::string inFile;
        std::string outFile;
    };

    void tokiConfigure(const Options &options);
    void tokiRun();
    void tokiShutdown();
    void tokiParse(const std::string &filename);
} // namespace TK
