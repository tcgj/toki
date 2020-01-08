#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "ioimage.hpp"

namespace IoImage {

    void releaseData(uChar** imageData) {
        stbi_image_free(*imageData);
        *imageData = nullptr;
    }

    int readFromFile(const char* filename, uChar** imageData,
                       int* imageWidth, int* imageHeight,
                       int* numComponents) {
        stbi_set_flip_vertically_on_load(true);

        int w, h, t;
        uChar* data = stbi_load(filename, &w, &h, &t, 0);

        if (data == nullptr) {
            fprintf(stderr, "Error: Image file cannot be read: %s\n", filename);
            return 0;
        }

        *imageData = data;
        *imageWidth = w;
        *imageHeight = h;
        *numComponents = t;
        return 1;
    }

    int writeToPngFile(const char* filename, const uChar* imageData,
                          int imageWidth, int imageHeight,
                          int numComponents) {
        stbi_flip_vertically_on_write(true);

        int result = stbi_write_png(filename, imageWidth, imageHeight,
                                    numComponents, imageData, 0);

        if (!result) {
            fprintf(stderr, "Error: Image file cannot be written: %s\n",
                    filename);
        }
        return result;
    }

    int writeToJpgFile(const char* filename, const uChar* imageData,
                          int imageWidth, int imageHeight, int numComponents,
                          int quality) {
        stbi_flip_vertically_on_write(true);

        int result = stbi_write_jpg(filename, imageWidth, imageHeight,
                                    numComponents, imageData, quality);

        if (!result) {
            fprintf(stderr, "Error: Image file cannot be written: %s\n",
                    filename);
        }
        return result;
    }
}  // namespace IoImage
