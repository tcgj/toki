#include "ioimage.hpp"
#include "math/vec3.hpp"

int main() {
    int width = 200;
    int height = 100;
    int num_components = 3;
    u_char* image_data = new u_char[width * height * num_components];
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int index = (j * width + i) * num_components;
            vec3 col(float(i) / float(width), float(j) / float(height), 0.2f);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            image_data[index] = ir;
            image_data[index + 1] = ig;
            image_data[index + 2] = ib;
        }
    }
    int result = ioimage::write_to_png_file("out.png", image_data, width,
                                            height, num_components);
    delete[] image_data;

    return 0;
}