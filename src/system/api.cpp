#include "api.hpp"

#include "appender.hpp"

// old
#include "stream.hpp"

// temp
#include "shape/mesh.hpp"
#include "camera/perspective.hpp"
#include "image/png.hpp"
#include "sampler/stratified.hpp"
#include "integrator/whitted.hpp"
#include "integrator/path.hpp"
#include "region/primitive.hpp"
#include "shape/triangle.hpp"
#include "shape/sphere.hpp"
#include "material/matte.hpp"
#include "material/mirror.hpp"
#include "core/spectrum.hpp"
#include "accelerator/bvh.hpp"
#include "accelerator/iterator.hpp"
#include "accelerator/bvh.hpp"
#include "light/point.hpp"
#include "light/directional.hpp"
#include "light/area.hpp"
#include "core/random.hpp"

namespace TK {
    TOKIContext g_Context;

    void RenderAPI::configure(const Options& options) {
        g_Context.m_Outfile = options.outfile;
        g_Context.m_ThreadCount = options.threadCount;
        g_Context.m_FastRender = options.fastRender;

        // Setup multi-threading
        auto scheduler = std::make_shared<Scheduler>();
        g_Context.setScheduler(scheduler);
        g_Context.setThreadPool(std::make_unique<ThreadPool>(g_Context.m_ThreadCount, scheduler));

        // Setup logger
#ifdef TK_DEBUG_MODE
        g_Context.setLogger(std::make_unique<Logger>(LEVEL_DEBUG));
#else
        g_Context.setLogger(std::make_unique<Logger>(LEVEL_INFO));
#endif
        Logger* logger = g_Context.logger();
        if (logger != nullptr)
            logger->addAppender(std::make_shared<StreamAppender>(&std::cout));
    }

    void RenderAPI::parse(std::string inputFile) {
        // TODO: Read required data from file

        // testScene(resolution, m_Outfile);

        // Cornell Box
        Point3f eye(278, 273, -800);
        Point3f at(278, 273, 0);
        Transform cameraToWorld = lookAt(eye, at, Vec3f(0, 1, 0));
        PNGImage output(g_Context.m_Resolution, g_Context.m_Outfile);
        auto camera =
            std::make_shared<PerspectiveCamera>(cameraToWorld, 1.0f, (at - eye).magnitude(), 40.0f, &output);
        // Round samplesPerPixel to nearest power of 2, then set to x/y, and get number of dimensions needed
        auto sampler = std::make_shared<StratifiedSampler>(8, 8, 0);

        tkSpectrum whiteKd(tkSpectrum::fromRGB(Vec3f(0.75f, 0.75f, 0.75f)));
        auto matteWhite = std::make_shared<Matte>(whiteKd);
        tkSpectrum redKd(tkSpectrum::fromRGB(Vec3f(0.66f, 0.0f, 0.0f)));
        auto matteRed = std::make_shared<Matte>(redKd);
        tkSpectrum greenKd(tkSpectrum::fromRGB(Vec3f(0.15f, 0.476f, 0.0f)));
        auto matteGreen = std::make_shared<Matte>(greenKd);
        tkSpectrum mirrorKd(tkSpectrum::fromRGB(Vec3f(0.9, 0.9, 0.9)));
        auto mirror = std::make_shared<Mirror>(mirrorKd);

        std::vector<std::shared_ptr<Primitive>> prims;

        // Lights
        std::vector<std::shared_ptr<Light>> lights;
        // lights.push_back(std::make_shared<DirectionalLight>(rotateY(degToRad(-90)), tkSpectrum(0.7)));
        // lights.push_back(std::make_shared<PointLight>(
        //     translate(Vec3f(343, 540, 227)), tkSpectrum(100000)));
        // lights.push_back(std::make_shared<PointLight>(
        //     translate(Vec3f(213, 540, 332)), tkSpectrum(100000)));
        Transform lightPos = translate(Vec3f(278, 548.8, 279.5));
        auto sphereLight = std::make_shared<Sphere>(&lightPos, 65);
        auto areaLight = std::make_shared<AreaLight>(Transform(), sphereLight, tkSpectrum(10.0f));
        lights.push_back(areaLight);
        prims.push_back(std::make_shared<Primitive>(sphereLight, matteWhite, areaLight));

        // Geometry
        Transform tf = translate(Vec3f::zero);
        Point3f V[64] = {
            // Floor 0-1
            Point3f(552.8, 0, 0), Point3f(0, 0, 0), Point3f(0, 0, 559.2), Point3f(549.6, 0, 559.2),
            // Light 36-37
            Point3f(343, 548.8, 227), Point3f(343, 548.8, 332), Point3f(213, 548.8, 332),
            Point3f(213, 548.8, 227),
            // Ceiling 2-5,32-35
            Point3f(556, 548.8, 0), Point3f(556, 548.8, 559.2), Point3f(0, 548.8, 559.2),
            Point3f(0, 548.8, 0),
            // Back wall 6-7
            Point3f(549.6, 0, 559.2), Point3f(0, 0, 559.2), Point3f(0, 548.8, 559.2),
            Point3f(556, 548.8, 559.2),
            // Right wall 8-9
            Point3f(0, 0, 559.2), Point3f(0, 0, 0), Point3f(0, 548.8, 0), Point3f(0, 548.8, 559.2),
            // Left wall 1011
            Point3f(552.8, 0, 0), Point3f(549.6, 0, 559.2), Point3f(556, 548.8, 559.2),
            Point3f(556, 548.8, 0),
            // Short block 12-21
            Point3f(130, 165, 65), Point3f(82, 165, 225), Point3f(240, 165, 272),
            Point3f(290, 165, 114),

            Point3f(290, 0, 114), Point3f(290, 165, 114), Point3f(240, 165, 272),
            Point3f(240, 0, 272),

            Point3f(130, 0, 65), Point3f(130, 165, 65), Point3f(290, 165, 114), Point3f(290, 0, 114),

            Point3f(82, 0, 225), Point3f(82, 165, 225), Point3f(130, 165, 65), Point3f(130, 0, 65),

            Point3f(240, 0, 272), Point3f(240, 165, 272), Point3f(82, 165, 225), Point3f(82, 0, 225),
            // Tall Block 22-31
            Point3f(423, 330, 247), Point3f(265, 330, 296), Point3f(314, 330, 456),
            Point3f(472, 330, 406),

            Point3f(423, 0, 247), Point3f(423, 330, 247), Point3f(472, 330, 406),
            Point3f(472, 0, 406),

            Point3f(472, 0, 406), Point3f(472, 330, 406), Point3f(314, 330, 456),
            Point3f(314, 0, 456),

            Point3f(314, 0, 456), Point3f(314, 330, 456), Point3f(265, 330, 296),
            Point3f(265, 0, 296),

            Point3f(265, 0, 296), Point3f(265, 330, 296), Point3f(423, 330, 247), Point3f(423, 0, 247)
        };
        int64_t I[114] = { 0,  1,  2,  0,  2,  3,  8,  4,  7,  8,  7,  11, 8,  9,  5,  8,  5,  4,  12,
                         13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23, 24, 25,
                         26, 24, 26, 27, 28, 29, 30, 28, 30, 31, 32, 33, 34, 32, 34, 35, 36, 37, 38,
                         36, 38, 39, 40, 41, 42, 40, 42, 43, 44, 45, 46, 44, 46, 47, 48, 49, 50, 48,
                         50, 51, 52, 53, 54, 52, 54, 55, 56, 57, 58, 56, 58, 59, 60, 61, 62, 60, 62,
                         63, 5,  9,  10, 5,  10, 6,  7,  6,  10, 7,  10, 11, 4,  5,  6,  4,  6,  7 };
        auto mesh = std::make_shared<Mesh>(tf, 64, 38, V, I, nullptr, nullptr);
        auto tri0 = std::make_shared<Triangle>(&tf, mesh, 0);
        auto tri1 = std::make_shared<Triangle>(&tf, mesh, 1);
        auto tri2 = std::make_shared<Triangle>(&tf, mesh, 2);
        auto tri3 = std::make_shared<Triangle>(&tf, mesh, 3);
        auto tri4 = std::make_shared<Triangle>(&tf, mesh, 4);
        auto tri5 = std::make_shared<Triangle>(&tf, mesh, 5);
        auto tri6 = std::make_shared<Triangle>(&tf, mesh, 6);
        auto tri7 = std::make_shared<Triangle>(&tf, mesh, 7);
        auto tri8 = std::make_shared<Triangle>(&tf, mesh, 8);
        auto tri9 = std::make_shared<Triangle>(&tf, mesh, 9);
        auto tri10 = std::make_shared<Triangle>(&tf, mesh, 10);
        auto tri11 = std::make_shared<Triangle>(&tf, mesh, 11);
        auto tri12 = std::make_shared<Triangle>(&tf, mesh, 12);
        auto tri13 = std::make_shared<Triangle>(&tf, mesh, 13);
        auto tri14 = std::make_shared<Triangle>(&tf, mesh, 14);
        auto tri15 = std::make_shared<Triangle>(&tf, mesh, 15);
        auto tri16 = std::make_shared<Triangle>(&tf, mesh, 16);
        auto tri17 = std::make_shared<Triangle>(&tf, mesh, 17);
        auto tri18 = std::make_shared<Triangle>(&tf, mesh, 18);
        auto tri19 = std::make_shared<Triangle>(&tf, mesh, 19);
        auto tri20 = std::make_shared<Triangle>(&tf, mesh, 20);
        auto tri21 = std::make_shared<Triangle>(&tf, mesh, 21);
        auto tri22 = std::make_shared<Triangle>(&tf, mesh, 22);
        auto tri23 = std::make_shared<Triangle>(&tf, mesh, 23);
        auto tri24 = std::make_shared<Triangle>(&tf, mesh, 24);
        auto tri25 = std::make_shared<Triangle>(&tf, mesh, 25);
        auto tri26 = std::make_shared<Triangle>(&tf, mesh, 26);
        auto tri27 = std::make_shared<Triangle>(&tf, mesh, 27);
        auto tri28 = std::make_shared<Triangle>(&tf, mesh, 28);
        auto tri29 = std::make_shared<Triangle>(&tf, mesh, 29);
        auto tri30 = std::make_shared<Triangle>(&tf, mesh, 30);
        auto tri31 = std::make_shared<Triangle>(&tf, mesh, 31);
        auto tri32 = std::make_shared<Triangle>(&tf, mesh, 32);
        auto tri33 = std::make_shared<Triangle>(&tf, mesh, 33);
        auto tri34 = std::make_shared<Triangle>(&tf, mesh, 34);
        auto tri35 = std::make_shared<Triangle>(&tf, mesh, 35);
        auto tri36 = std::make_shared<Triangle>(&tf, mesh, 36);
        auto tri37 = std::make_shared<Triangle>(&tf, mesh, 37);
        auto areaLight1 = std::make_shared<AreaLight>(tf, tri36, tkSpectrum(50.0f));
        auto areaLight2 = std::make_shared<AreaLight>(tf, tri37, tkSpectrum(50.0f));
        prims.push_back(std::make_shared<Primitive>(tri0, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri1, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri2, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri3, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri4, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri5, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri6, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri7, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri8, matteGreen));
        prims.push_back(std::make_shared<Primitive>(tri9, matteGreen));
        prims.push_back(std::make_shared<Primitive>(tri10, matteRed));
        prims.push_back(std::make_shared<Primitive>(tri11, matteRed));
        /*prims.push_back(std::make_shared<Primitive>(tri12, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri13, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri14, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri15, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri16, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri17, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri18, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri19, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri20, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri21, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri22, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri23, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri24, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri25, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri26, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri27, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri28, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri29, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri30, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri31, matteWhite));*/
        prims.push_back(std::make_shared<Primitive>(tri32, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri33, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri34, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri35, matteWhite));
        prims.push_back(std::make_shared<Primitive>(tri36, matteWhite, areaLight1));
        prims.push_back(std::make_shared<Primitive>(tri37, matteWhite, areaLight2));

        /*Transform sphereTf = translate(Vec3f(185, 397, 353));
        auto mirrorSphere = std::make_shared<Sphere>(&sphereTf, 103);
        prims.push_back(std::make_shared<Primitive>(mirrorSphere, mirror));*/

        // lights.push_back(areaLight1);
        // lights.push_back(areaLight2);

        // Accel Structure
        // auto accel = std::make_shared<Iterator>(prims, AABBf(Point3f(0), Point3f(560, 560, -560)));
        auto accel = std::make_shared<BVH>(prims);


        // WhittedIntegrator integrator(3, camera, sampler);
        auto integrator = std::make_shared<PathTracingIntegrator>(20);

        // Scene
        Scene scene(accel, lights, camera, integrator);
        {
            using clock = std::chrono::system_clock;
            using ms = std::chrono::duration<double, std::milli>;

            const auto before = clock::now();
            // integrator.render(scene);
            const ms duration = clock::now() - before;

            std::cout << "Process took: " << duration.count() << "ms\n";
        }
    }

    void testScene(const Vec2i& resolution, std::string outfile) {
        // Initialise camera
        Point3f eye(3.0f, 5.0f, 5.0f);
        Point3f at(0.0f, 2.0f, 0.0f);
        Transform cameraToWorld = lookAt(eye, at, Vec3f(0, 1, 0));
        PNGImage output(resolution, outfile);
        auto camera =
            std::make_shared<PerspectiveCamera>(cameraToWorld, 1.0f, (at - eye).magnitude(), 60.0f, &output);
        // Set up sampler
        auto sampler = std::make_shared<StratifiedSampler>(4, 4, 5);

        // Materials
        tkSpectrum greyKd(tkSpectrum::fromRGB(Vec3f(0.9f, 0.9f, 0.9f)));
        auto matteGrey = std::make_shared<Matte>(greyKd);
        tkSpectrum redKd(tkSpectrum::fromRGB(Vec3f(0.9f, 0.0f, 0.0f)));
        auto matteRed = std::make_shared<Matte>(redKd);
        tkSpectrum blueKd(tkSpectrum::fromRGB(Vec3f(0.01f, 0.77f, 0.98f)));
        auto matteBlue = std::make_shared<Matte>(blueKd);
        tkSpectrum yellowKd(tkSpectrum::fromRGB(Vec3f(1.0f, 1.0f, 0.0f)));
        auto matteYellow = std::make_shared<Matte>(yellowKd);

        // Primitives
        std::vector<std::shared_ptr<Primitive>> prims;

        // Plane
        Transform tf = translate(Vec3f::zero);
        int64_t I[6] = { 0, 1, 3, 1, 2, 3 };
        Point3f V[4] = { Point3f(-10, 0, 10), Point3f(10, 0, 10), Point3f(10, 0, -10),
                           Point3f(-10, 0, -10) };
        Vec3f N[4] = { Vec3f(0, 1, 0), Vec3f(0, 1, 0), Vec3f(0, 1, 0), Vec3f(0, 1, 0) };
        auto mesh = std::make_shared<Mesh>(tf, 4, 2, V, I, N, nullptr);
        auto tri1 = std::make_shared<Triangle>(&tf, mesh, 0);
        auto tri2 = std::make_shared<Triangle>(&tf, mesh, 1);
        prims.push_back(std::make_shared<Primitive>(tri1, matteGrey));
        prims.push_back(std::make_shared<Primitive>(tri2, matteYellow));

        // Sphere
        Transform sphereToWorld = translate(Vec3f(1.0f, 2.0f, 0.0f));
        auto sphere = std::make_shared<Sphere>(&sphereToWorld, 1.0f);
        prims.push_back(std::make_shared<Primitive>(sphere, matteRed));

        // Lights
        std::vector<std::shared_ptr<Light>> lights;
        // lights.push_back(std::make_shared<DirectionalLight>(rotateZ(degToRad(-90)),
        // tkSpectrum(0.4)));
        // lights.push_back(std::make_shared<PointLight>(translate(Vec3f(-2.0f,
        // 4.0f, 0)), tkSpectrum(10)));
        Transform lightPos = translate(Vec3f(-3.0f, 6.0f, 0.0f));
        auto sphereLight = std::make_shared<Sphere>(&lightPos, 2.0f);
        auto areaLight = std::make_shared<AreaLight>(Transform(), sphereLight, tkSpectrum(1.0f));
        lights.push_back(areaLight);
        prims.push_back(std::make_shared<Primitive>(sphereLight, nullptr, areaLight));

        // Accel Structure
        auto accel = std::make_shared<BVH>(prims);

        auto integrator = std::make_shared<WhittedIntegrator>(3);

        // Scene
        Scene scene(accel, lights, camera, integrator);

        // integrator.render(scene);
    }

    void RenderAPI::run() {}

    void RenderAPI::shutdown() {
        ThreadPool* pool = g_Context.threadpool();
        if (pool != nullptr)
            pool->shutdown();
    }
}  // namespace TK
