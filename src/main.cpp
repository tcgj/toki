/*
#include "system/io/ioimage.hpp"
#include "math/math.hpp"
#include "petershirley/camera/camera.hpp"
#include "petershirley/material/lambertian.hpp"
#include "petershirley/material/metal.hpp"
#include "petershirley/primitive/intersectablelist.hpp"
#include "petershirley/primitive/sphere.hpp"
#include "system/system.hpp"

tkVec3f color(const Ray& r, Intersectable& world, int depth) {
    RayInteraction ri;
    if (world.intersects(r, 0.001f, MAXFLOAT, ri)) {
        Ray scattered;
        tkVec3f attn;
        if (depth < 50 && ri.matPtr->computeScattering(r, ri, attn, scattered)) {
            return attn * color(scattered, world, depth + 1);
        } else {
            return tkVec3f::zero;
        }
    }

    tkVec3f unit_dir = unitVectorOf(r.d);
    float t = 0.5f * (unit_dir.y + 1.0f);
    return (1.0f - t) * tkVec3f::one + t * tkVec3f(0.5f, 0.7f, 1.0f);
}

int main() {
    Random::seed(0);
    int width = 1280;
    int height = 768;
    int numSamples = 20;
    int numComponents = 3;
    uChar* imageData = new uChar[width * height * numComponents];

    tkVec3f up(0.0f, 1.0f, 0.0f);
    tkPoint3f origin(13.0f, 2.0f, 3.0f);
    tkPoint3f lookat(0.0f, 0.0f, 0.0f);

    int n = 30;
    Intersectable** list = new Intersectable*[n + 1];
    list[0] = new Sphere(tkPoint3f(0, -1000, 0), 1000,
                         new Lambertian(tkVec3f(0.5f, 0.5f, 0.5f)));
    int i = 1;
    for (int a = 0; a < 5; a++) {
        for (int b = 0; b < 5; b++) {
            float chooseMat = Random::nextFloat();
            tkPoint3f center(a + 0.9f * Random::nextFloat(), 0.2f,
                        b + 0.9f * Random::nextFloat());
            if ((center - tkPoint3f(4, 0.2f, 0)).magnitude() > 0.9f) {
                if (chooseMat < 0.8f) {  // diffuse
                    list[i++] = new Sphere(
                        center, 0.2,
                        new Lambertian(
                            tkVec3f(Random::nextFloat() * Random::nextFloat(),
                                 Random::nextFloat() * Random::nextFloat(),
                                 Random::nextFloat() * Random::nextFloat())));
                } else {  // metal
                    list[i++] = new Sphere(
                        center, 0.2,
                        new Metal(tkVec3f(0.5f * (1 + Random::nextFloat()),
                                       0.5f * (1 + Random::nextFloat()),
                                       0.5f * (1 + Random::nextFloat())),
                                  0.5f * Random::nextFloat()));
                }
            }
        }
    }
    list[i++] = new Sphere(tkPoint3f(0, 1, 0), 1.0f, new Lambertian(tkVec3f(0.4f, 0.2f, 0.1f)));
    list[i++] = new Sphere(tkPoint3f(4, 1, 0), 1.0f, new Metal(tkVec3f(0.7f, 0.6f, 0.5f), 0.0f));
    IntersectableList world(list, i);
    Camera camera(origin, lookat, up, 20.0f, float(width) / height, 10);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int index = (j * width + i) * numComponents;
            tkVec3f col(0, 0, 0);
            for (int s = 0; s < numSamples; s++) {
                float u = float(i + Random::nextFloat()) / float(width);
                float v = float(j + Random::nextFloat()) / float(height);
                Ray r = camera.raycastTo(u, v);
                col += color(r, world, 0);
            }
            col /= float(numSamples);
            col = tkVec3f(sqrtf(col.r), sqrtf(col.g), sqrtf(col.b));
            int ir = int(255.99f * col[0]);
            int ig = int(255.99f * col[1]);
            int ib = int(255.99f * col[2]);
            imageData[index] = ir;
            imageData[index + 1] = ig;
            imageData[index + 2] = ib;
        }
    }
    int result = IoImage::writeToPngFile("out.png", imageData, width, height,
                                         numComponents);
    delete[] imageData;

    return 0;
}
*/
#include "system/system.hpp"
#include "math/math.hpp"
#include "math/matrix44.hpp"
#include "geometry/ray.hpp"
#include "geometry/aabb.hpp"
#include "geometry/geometry.hpp"
#include "geometry/quaternion.hpp"
#include "geometry/transform.hpp"
#include "region/primitive.hpp"
#include "spectrum/rgbspectrum.hpp"
#include "shape/curve.hpp"
#include "shape/sphere.hpp"
#include "shape/triangle.hpp"
#include "interaction/surfaceinteraction.hpp"
#include "accelerator/bvh.hpp"
#include "camera/orthographic.hpp"

int main() {
    return 0;
}