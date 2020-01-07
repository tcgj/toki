#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include "ioimage.hpp"

namespace ioimage {

    void release_data(u_char** image_data) {
        stbi_image_free(*image_data);
        *image_data = nullptr;
    }

    int read_from_file(const char* filename, u_char** image_data,
                       int* image_width, int* image_height,
                       int* num_components) {
        stbi_set_flip_vertically_on_load(true);

        int w, h, t;
        u_char* data = stbi_load(filename, &w, &h, &t, 0);

        if (data == nullptr) {
            fprintf(stderr, "Error: Image file cannot be read: %s\n", filename);
            return 0;
        }

        *image_data = data;
        *image_width = w;
        *image_height = h;
        *num_components = t;
        return 1;
    }

    int write_to_png_file(const char* filename, const u_char* image_data,
                          int image_width, int image_height,
                          int num_components) {
        stbi_flip_vertically_on_write(true);

        int result = stbi_write_png(filename, image_width, image_height,
                                    num_components, image_data, 0);

        if (!result) {
            fprintf(stderr, "Error: Image file cannot be written: %s\n",
                    filename);
        }
        return result;
    }

    int write_to_jpg_file(const char* filename, const u_char* image_data,
                          int image_width, int image_height, int num_components,
                          int quality) {
        stbi_flip_vertically_on_write(true);

        int result = stbi_write_jpg(filename, image_width, image_height,
                                    num_components, image_data, quality);

        if (!result) {
            fprintf(stderr, "Error: Image file cannot be written: %s\n",
                    filename);
        }
        return result;
    }
}  // namespace ioimage
