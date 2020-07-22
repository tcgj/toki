#include "api.hpp"

namespace TK {
    Options tokiOptions;

    void tokiConfigure(const Options &options) {
        tokiOptions = options;


        // Initialize any other systems
        initThreads();
    }

    void tokiRun() {}

    void tokiShutdown() {}

    void tokiParse(const std::string &filename) {}
}  // namespace TK
