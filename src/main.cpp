#include "ioimage.hpp"
#include "camera/camera.hpp"
#include "primitive/sphere.hpp"
#include "primitive/intersectablelist.hpp"
#include "random/random.hpp"

Vec3 color(const Ray& r, Intersectable& world) {
    RayInteraction ri;
    if (world.intersects(r, 0.0, MAXFLOAT, ri)) {
        return 0.5f * (ri.normal + Vec3::one);
    }

    Vec3 unit_dir = unitVectorOf(r.direction);
    float t = 0.5f * (unit_dir.y + 1.0f);
    return (1.0f - t) * Vec3::one + t * Vec3(0.5f, 0.7f, 1.0f);
}

int main() {
    Random::seed(0);
    int width = 200;
    int height = 100;
    int numSamples = 100;
    int numComponents = 3;
    uChar* imageData = new uChar[width * height * numComponents];

    Vec3 up(0.0f, 1.0f, 0.0f);
    Point3 origin(0.0f, 0.0f, 0.0f);
    Point3 lookat(0.0f, 0.0f, -1.0f);

    Intersectable* list[] = {
        new Sphere(Point3(0, 0, -1), 0.5f),
        new Sphere(Point3(0, -100.5f, -1), 100)
    };
    IntersectableList world(list, 2);
    Camera camera(origin, lookat, up, 90.0f, float(width) / height, 1.0f);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int index = (j * width + i) * numComponents;
            Vec3 col(0, 0, 0);
            for (int s = 0; s < numSamples; s++) {
                float u = float(i + Random::nextFloat()) / float(width);
                float v = float(j + Random::nextFloat()) / float(height);
                Ray r = camera.raycastTo(u, v);
                col += color(r, world);
            }
            col /= float(numSamples);
            int ir = int(255.99f * col[0]);
            int ig = int(255.99f * col[1]);
            int ib = int(255.99f * col[2]);
            imageData[index] = ir;
            imageData[index + 1] = ig;
            imageData[index + 2] = ib;
        }
    }
    int result = IoImage::writeToPngFile("out.png", imageData, width,
                                            height, numComponents);
    delete[] imageData;

    return 0;
}