#include "imageio.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace TK {
    void releaseData(unsigned char** imageData) {
        stbi_image_free(*imageData);
        *imageData = nullptr;
    }

    int readFromFile(const char* filename, unsigned char** imageData, int* imageWidth, int* imageHeight,
                       int* numComponents) {
        stbi_set_flip_vertically_on_load(true);

        int w;
        int h;
        int t;
        unsigned char* data = stbi_load(filename, &w, &h, &t, 0);

        if (data == nullptr) {
            fprintf(stderr, "Error: Image file cannot be read: %s\n", filename);
            return 1;
        }

        *imageData = data;
        *imageWidth = w;
        *imageHeight = h;
        *numComponents = t;
        return 0;
    }

    int writeToPngFile(const char* filename, const unsigned char* imageData, int imageWidth,
                         int imageHeight, int numComponents) {
        stbi_flip_vertically_on_write(true);

        int result = stbi_write_png(filename, imageWidth, imageHeight, numComponents, imageData, 0);

        if (!result) {
            fprintf(stderr, "Error: Image file cannot be written: %s\n", filename);
            return 1;
        }
        return 0;
    }

    int writeToJpgFile(const char* filename, const unsigned char* imageData, int imageWidth,
                         int imageHeight, int numComponents, int quality) {
        stbi_flip_vertically_on_write(true);

        int result = stbi_write_jpg(filename, imageWidth, imageHeight, numComponents, imageData, quality);

        if (!result) {
            fprintf(stderr, "Error: Image file cannot be written: %s\n", filename);
            return 1;
        }
        return 0;
    }
}  // namespace TK
