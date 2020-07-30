#include "api.hpp"

#include "stream.hpp"

// temp
#include "shape/mesh.hpp"
#include "camera/perspective.hpp"
#include "image/png.hpp"
#include "sampler/stratified.hpp"
#include "integrator/whitted.hpp"
#include "region/primitive.hpp"
#include "shape/triangle.hpp"
#include "shape/sphere.hpp"
#include "material/matte.hpp"
#include "core/spectrum.hpp"
#include "accelerator/bvh.hpp"
#include "light/point.hpp"
#include "light/directional.hpp"
#include "light/area.hpp"

namespace TK {
    static Options tokiOptions;

    void tokiConfigure(const Options &cmdLineOptions) {
        tokiOptions = cmdLineOptions;

        // Initialize any other systems
        initThreads(tokiOptions.threadCount);
    }

    void tokiParse(IStream &stream) {
        // Read configurations
        stream >> tokiOptions.tileSize;
        stream >> tokiOptions.samplesPerPixel;
        stream >> tokiOptions.resolution;

        // TODO: Read required data from stream

        // Resolution default
        tokiOptions.resolution = tkVec2i(1024, 768);

        // Initialise camera
        tkPoint3f eye(3.0f, 5.0f, 5.0f);
        tkPoint3f at(0.0f, 2.0f, 0.0f);
        Transform cameraToWorld = lookAt(eye, at, tkVec3f(0, 1, 0));
        PNGImage output(tokiOptions.resolution, tokiOptions.outFile);
        auto camera = std::make_shared<PerspectiveCamera>(
            cameraToWorld, 1.0f, (at - eye).magnitude(), 60.0f, &output);
        // Set up sampler
        auto sampler = std::make_shared<StratifiedSampler>();

        // Materials
        tkSpectrum greyKd(tkSpectrum::fromRGB(tkVec3f(0.9f, 0.9f, 0.9f)));
        auto matteGrey = std::make_shared<Matte>(greyKd);
        tkSpectrum redKd(tkSpectrum::fromRGB(tkVec3f(0.9f, 0.0f, 0.0f)));
        auto matteRed = std::make_shared<Matte>(redKd);
        tkSpectrum blueKd(tkSpectrum::fromRGB(tkVec3f(0.01f, 0.77f, 0.98f)));
        auto matteBlue = std::make_shared<Matte>(blueKd);
        tkSpectrum yellowKd(tkSpectrum::fromRGB(tkVec3f(1.0f, 1.0f, 0.0f)));
        auto matteYellow = std::make_shared<Matte>(yellowKd);

        // Primitives
        std::vector<std::shared_ptr<Primitive>> prims;

        // Plane
        Transform tf = translate(tkVec3f::zero);
        tkUInt I[6] = {0, 1, 3, 1, 2, 3};
        tkPoint3f V[4] = { tkPoint3f(-10, 0, 10), tkPoint3f(10, 0, 10), tkPoint3f(10, 0, -10), tkPoint3f(-10, 0, -10) };
        tkVec3f N[4] = { tkVec3f(0, 1, 0), tkVec3f(0, 1, 0), tkVec3f(0, 1, 0), tkVec3f(0, 1, 0) };
        auto mesh = std::make_shared<Mesh>(tf, 2, I, 4, V, N, nullptr);
        auto tri1 = std::make_shared<Triangle>(&tf, mesh, 0);
        auto tri2 = std::make_shared<Triangle>(&tf, mesh, 1);
        prims.push_back(std::make_shared<Primitive>(tri1, matteGrey));
        prims.push_back(std::make_shared<Primitive>(tri2, matteYellow));

        // Sphere
        Transform sphereToWorld = translate(tkVec3f(1.0f, 2.0f, 0.0f));
        auto sphere = std::make_shared<Sphere>(&sphereToWorld, 1.0f);
        prims.push_back(std::make_shared<Primitive>(sphere, matteRed));

        // Accel Structure
        auto accel = std::make_shared<BVH>(prims);

        // Lights
        std::vector<std::shared_ptr<Light>> lights;
        // lights.push_back(std::make_shared<DirectionalLight>(rotateZ(degToRad(-90)), tkSpectrum(0.4)));
        // lights.push_back(std::make_shared<PointLight>(translate(tkVec3f(-2.0f, 4.0f, 0)), tkSpectrum(10)));
        Transform lightPos = translate(tkVec3f(0.0f, 6.0f, 0.0f));
        auto sphereLight = std::make_shared<Sphere>(&lightPos, 2.0f);
        lights.push_back(std::make_shared<AreaLight>(Transform(), sphereLight, tkSpectrum(1.0f)));

        // Scene
        Scene scene(accel, lights);

        WhittedIntegrator integrator(3, camera, sampler);
        integrator.render(scene);
    }

    void tokiRun() {
    }

    void tokiShutdown() {
        cleanupThreads();
    }
}  // namespace TK
