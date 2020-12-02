#pragma once

#include "system/toki.hpp"
#include "math/math.hpp"
#include "core/parallel.hpp"

namespace TK {
    struct Options {
        std::string outFile;
        tkInt threadCount = -1;
        bool fastRender = false;
    };

    class RenderAPI {
    public:
        static void tokiConfigure(const Options& options);
        static void tokiParse(std::string inputFile);
        static void tokiRun();
        static void tokiShutdown();

    private:
        // Options
        static std::string outFile;
        static tkInt threadCount;
        static bool fastRender;

        // Render Settings
        static tkInt tileSize;
        static tkInt samplesPerPixel;
        static tkVec2i resolution;
    };

    // Serializing API
}  // namespace TK
