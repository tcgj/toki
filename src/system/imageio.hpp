#pragma once

#include "toki.hpp"

namespace TK {
    // Frees image data from file read.
    void releaseData(unsigned char** imageData);

    // Reads an image given the filename. Returns 0 if file is read, and 1
    // otherwise.
    //
    // Resulting image data is pointed to by imageData, width pointed to by
    // imageWidth, height pointed to by imageHeight, and the number of
    // components pointed to by numComponents.
    //
    // Image data begins from bottom-left of image.
    int readFromFile(const char* filename, unsigned char** imageData, int* imageWidth, int* imageHeight,
                       int* numComponents);

    // Writes an image to file given the filename, in the PNG format. Returns 0
    // if file is written, and 1 otherwise.
    //
    // Image data should be provided in imageData, width in imageWidth, height
    // in imageHeight, and the number of components in numComponents.
    //
    // Image data should begin from bottom-left of image.
    int writeToPngFile(const char* filename, const unsigned char* imageData, int imageWidth,
                         int imageHeight, int numComponents);

    // Writes an image to file given the filename, in the JPG format. Returns 0
    // if file is written, and 1 otherwise.
    //
    // Image data should be provided in imageData, width in imageWidth, height
    // in imageHeight, and the number of components in numComponents.
    // Quality is an integer from 0 to 100.
    //
    // Image data should begin from bottom-left of image.
    int writeToJpgFile(const char* filename, const unsigned char* imageData, int imageWidth,
                         int imageHeight, int numComponents, int quality);
}  // namespace TK
