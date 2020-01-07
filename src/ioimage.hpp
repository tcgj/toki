#ifndef IO_IMAGE_HPP
#define IO_IMAGE_HPP

typedef unsigned char u_char;

namespace ioimage {

    // Frees image data from file read.
    void release_data(u_char** imageData);

    // Reads an image given the filename. Returns 1 if file is read, and 0
    // otherwise.
    //
    // Resulting image data is pointed to by image_data, width pointed to by
    // image_width, height pointed to by image_height, and the number of
    // components pointed to by num_components.
    //
    // Image data begins from bottom-left of image.
    int read_from_file(const char* filename, u_char** image_data,
                       int* image_width, int* image_height,
                       int* num_components);

    // Writes an image to file given the filename, in the PNG format. Returns 1
    // if file is written, and 0 otherwise.
    //
    // Image data should be provided in image_data, width in image_width, height
    // in image_height, and the number of components in num_components.
    //
    // Image data should begin from bottom-left of image.
    int write_to_png_file(const char* filename, const u_char* image_data,
                          int image_width, int image_height,
                          int num_components);

    // Writes an image to file given the filename, in the JPG format. Returns 1
    // if file is written, and 0 otherwise.
    //
    // Image data should be provided in image_data, width in image_width, height
    // in image_height, and the number of components in num_components.
    // Quality is an integer from 0 to 100.
    //
    // Image data should begin from bottom-left of image.
    int write_to_jpg_file(const char* filename, const u_char* image_data,
                          int image_width, int image_height, int num_components,
                          int quality);
}  // namespace ioimage

#endif  // IO_IMAGE_HPP
