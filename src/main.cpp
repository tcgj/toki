#include "ioimage.hpp"
#include "math/math.hpp"
#include "ray.hpp"

float hitSphere(const Point3& center, float radius, const Ray& r) {
    Vec3 c_o = r.origin - center;
    float c = dot(c_o, c_o) - radius * radius;
    float b = dot(r.direction, c_o) * 2.0f;
    float a = dot(r.direction, r.direction);
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return -1.0f;
    } else {
        return (-b - sqrtf(discriminant)) / (2.0f * a);
    }
}

Vec3 color(const Ray& r) {
    float t = hitSphere(Point3(0.0f, 0.0f, -1.0f), 0.5f, r);
    if (t > 0) {
        Vec3 n = Vec3(r(t) - Vec3(0.0f, 0.0f, -1.0f)).normalize();
        return 0.5f * (Vec3(n.x, n.y, n.z) + Vec3::one);
    }
    Vec3 unit_dir = unitVectorOf(r.direction);
    t = 0.5f * (unit_dir.y + 1.0f);
    return (1.0f - t) * Vec3::one + t * Vec3(0.5f, 0.7f, 1.0f);
}

int main() {
    int width = 200;
    int height = 100;
    int numComponents = 3;
    uChar* imageData = new uChar[width * height * numComponents];
    Vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
    Vec3 horizontal(4.0f, 0.0f, 0.0f);
    Vec3 vertical(0.0f, 2.0f, 0.0f);
    Point3 origin(0.0f, 0.0f, 0.0f);
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            int index = (j * width + i) * numComponents;
            float u = float(i) / float(width);
            float v = float(j) / float(height);
            Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical);
            Vec3 col = color(r);
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