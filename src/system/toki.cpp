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
                options.outfile = argv[i];
            } else if (strcmp(str, "-f")) {
                options.fastRender = true;
            } else {
                inputFile = argv[i];
            }
        }

        // Pre-init
        std::cout << "--toki--\n\n" << "Copyright (c) 2020 tcgj\n";
#ifdef TK_DEBUG_MODE
        std::cout << "This build is currently running in debug mode\n";
#endif

        // Init
        RenderAPI::configure(options);
        RenderAPI::parse(inputFile);

        // Run
        RenderAPI::run();

        // Cleanup
        RenderAPI::shutdown();
        return 0;
    }
}  // namespace TK

int main(int argc, char* argv[]) {
    return TK::startToki(argc, argv);
}
