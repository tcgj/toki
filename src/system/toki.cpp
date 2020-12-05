#include "toki.hpp"

#include "api.hpp"

namespace TK {
    void printUsage(const char* message = nullptr) {
        if (message != nullptr)
            fprintf(stderr, "toki: %s\n", message);
        exit(message == nullptr ? 0 : 1);
    }

    int startToki(int argc, char* argv[]) {
        Options options;
        std::string inputFile;
        for (int i = 1; i < argc; ++i) {
            char* str = argv[i];
            if (strcmp(str, "-nt") == 0) {
                i++;
                if (i == argc)
                    printUsage("missing numerical value after -nt");
                options.threadCount = atoi(argv[i]);
            } else if (strcmp(str, "-o") == 0) {
                i++;
                if (i == argc)
                    printUsage("missing filename after -o");
                options.outFile = argv[i];
            } else if (strcmp(str, "-f")) {
                options.fastRender = true;
            } else {
                inputFile = argv[i];
            }
        }

        // Pre-init
        std::cout << "--toki--" << std::endl << "Copyright (c) 2020 tcgj" << std::endl;
#ifdef TK_DEBUG_MODE
        std::cout << "This build is currently running in debug mode" << std::endl;
#endif

        // Init
        RenderAPI::tokiConfigure(options);
        RenderAPI::tokiParse(inputFile);

        // Run
        RenderAPI::tokiRun();

        // Cleanup
        RenderAPI::tokiShutdown();
        return 0;
    }
}  // namespace TK

int main(int argc, char* argv[]) {
    return TK::startToki(argc, argv);
}
