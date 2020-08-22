#include "imageio.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace TK {
    void releaseData(tkUChar **imageData) {
        stbi_image_free(*imageData);
        *imageData = nullptr;
    }

    tkInt readFromFile(const tkChar *filename, tkUChar **imageData,
                    tkInt *imageWidth, tkInt *imageHeight,
                    tkInt *numComponents) {
        stbi_set_flip_vertically_on_load(true);

        tkInt w;
        tkInt h;
        tkInt t;
        tkUChar *data = stbi_load(filename, &w, &h, &t, 0);

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

    tkInt writeToPngFile(const tkChar *filename, const tkUChar *imageData,
                        tkInt imageWidth, tkInt imageHeight,
                        tkInt numComponents) {
        stbi_flip_vertically_on_write(true);

        tkInt result = stbi_write_png(filename, imageWidth, imageHeight,
                                    numComponents, imageData, 0);

        if (!result) {
            fprintf(stderr, "Error: Image file cannot be written: %s\n",
                    filename);
            return 1;
        }
        return 0;
    }

    tkInt writeToJpgFile(const tkChar *filename, const tkUChar *imageData,
                        tkInt imageWidth, tkInt imageHeight, tkInt numComponents,
                        tkInt quality) {
        stbi_flip_vertically_on_write(true);

        tkInt result = stbi_write_jpg(filename, imageWidth, imageHeight,
                                    numComponents, imageData, quality);

        if (!result) {
            fprintf(stderr, "Error: Image file cannot be written: %s\n",
                    filename);
            return 1;
        }
        return 0;
    }
} // namespace TK
