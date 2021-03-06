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
            const char* str = argv[i];
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
            } else if (strcmp(str, "-f") == 0) {
                options.fastRender = true;
            } else if (strcmp(str, "-g") == 0) {
                options.gui = true;
            } else {
                if (!inputFile.empty())
                    printUsage("more arguments provided than expected");

                inputFile = str;
                size_t extPos = inputFile.find_last_of(".");
                if (extPos == std::string::npos)
                    printUsage("missing file extension");

                if (inputFile.substr(extPos) == ".obj") // Wavefront .obj file
                    options.inputType = OBJ;
            }
        }

        // Pre-init
        std::cout << "---------- toki ----------\n\n" << "Copyright (c) 2020 tcgj\n";
#ifdef TK_DEBUG_MODE
        std::cout << "This build is currently running in debug mode\n";
#endif

        {
            using clock = std::chrono::system_clock;
            using ms = std::chrono::duration<double>;

            const auto before = clock::now();

            // Init
            RenderAPI::configure(options);
            RenderAPI::parse(inputFile);

            // Run
            RenderAPI::render();

            // Cleanup
            RenderAPI::shutdown();

            const ms duration = clock::now() - before;

            std::cout << "Process took: " << duration.count() << "s\n";
        }
        return 0;
    }
}  // namespace TK

int main(int argc, char* argv[]) {
    return TK::startToki(argc, argv);
}
