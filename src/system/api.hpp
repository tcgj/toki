#pragma once

#include "toki.hpp"

namespace TK {
    struct Options {
        std::string outFile;
        int threadCount = -1;
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
        static int threadCount;
        static bool fastRender;

        // Render Settings
        static int tileSize;
        static int samplesPerPixel;
        static Vec2i resolution;
    };

    // Serializing API
}  // namespace TK
