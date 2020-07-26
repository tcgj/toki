#pragma once

#include "system/toki.hpp"
#include "math/math.hpp"
#include "core/parallel.hpp"

namespace TK {
    struct Options {
        // Command-line options
        std::string outFile;
        tkInt threadCount = -1;
        bool fastRender = false;

        // File-specific options
        tkInt tileSize;
        tkInt samplesPerPixel;
        tkVec2i resolution;
    };

    void tokiConfigure(const Options &options);
    void tokiRun();
    void tokiShutdown();

    // Serializing API
} // namespace TK
