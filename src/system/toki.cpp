#include "toki.hpp"

#include "api.hpp"

namespace TK {
    void printUsage(const char *message = nullptr) {
        if (message != nullptr)
            fprintf(stderr, "toki: ");
        exit(message == nullptr ? 0 : 1);
    }

    tkInt startToki(tkInt argc, tkChar *argv[]) {
        TK::Options options;
        for (int i = 1; i < argc; ++i) {
            char *str = argv[i];
            if (strcmp(str, "-nt")) {
                i++;
                if (i == argc)
                    printUsage("missing numerical value after -nt");
                options.threadCount = atoi(argv[i]);
            } else if (strcmp(str, "-o")) {
                i++;
                if (i == argc)
                    printUsage("missing filename after -o");
                options.outFile = argv[i];
            } else {
                options.inFile = argv[i];
            }
        }

        // Pre-init
        std::cout << "--toki--" << std::endl << "Copyright (c) 2020 tcgj" << std::endl;
#ifdef TK_DEBUG_MODE
        std::cout << "This build is currently running in debug mode" << std::endl;
#endif

        // Init
        tokiConfigure(options);

        // Run
        tokiRun();

        // Cleanup
        tokiShutdown();
        return 0;
    }
} // namespace TK
