#include "api.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "appender.hpp"
#include "formatter.hpp"

// temp
#include "shape/mesh.hpp"
#include "camera/perspective.hpp"
#include "image/png.hpp"
#include "sampler/stratified.hpp"
#include "integrator/whitted.hpp"
#include "integrator/path.hpp"
#include "integrator/ao.hpp"
#include "region/primitive.hpp"
#include "shape/triangle.hpp"
#include "shape/sphere.hpp"
#include "material/diffuse.hpp"
#include "core/spectrum.hpp"
#include "accelerator/bvh.hpp"
#include "accelerator/iterator.hpp"
#include "accelerator/bvh.hpp"
#include "light/point.hpp"
#include "light/distant.hpp"
#include "light/area.hpp"
#include "core/random.hpp"

namespace TK {
    void RenderAPI::configure(const Options& options) {
        g_Context.m_Outfile = options.outfile;
        g_Context.m_ThreadCount = options.threadCount;
        g_Context.m_FastRender = options.fastRender;
        g_Context.m_Gui = options.gui;
        g_Context.m_InputType = options.inputType;

        // Setup logger
#ifdef TK_DEBUG_MODE
        g_Context.setLogger(std::make_unique<Logger>(LEVEL_DEBUG));
#else
        g_Context.setLogger(std::make_unique<Logger>(LEVEL_INFO));
#endif
        Logger* logger = g_Context.logger();
        if (logger != nullptr) {
            logger->addAppender(std::make_shared<StreamAppender>(&std::cout));
            logger->setFormatter(std::make_shared<Formatter>());
        }

        // Setup multi-threading
        auto scheduler = std::make_shared<Scheduler>();
        g_Context.setScheduler(scheduler);
        g_Context.setThreadPool(std::make_unique<ThreadPool>(g_Context.m_ThreadCount, scheduler));
    }

    void parseObj(const std::string& inputFile) {
        tinyobj::ObjReaderConfig readerConfig;
        tinyobj::ObjReader reader;
        readerConfig.triangulate = true;
        readerConfig.vertex_color = false;

        if (!reader.ParseFromFile(inputFile, readerConfig))
            LOG(LEVEL_ERROR, "TinyObjReader: %s", reader.Error().c_str());

        if (!reader.Warning().empty())
            LOG(LEVEL_WARNING, "TinyObjReader: %s", reader.Warning().c_str());

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();

        size_t numVert = 0;
        for (const auto& s : shapes) {
            numVert += s.mesh.num_face_vertices.size() * 3;
        }

        std::vector<Point3f> V;
        V.reserve(numVert);
        std::vector<int64_t> I;
        I.reserve(numVert);
        std::vector<Vec3f> N;
        N.reserve(numVert);
        std::vector<Point2f> UV;
        UV.reserve(numVert);

        for (const auto& s : shapes) {
            size_t indexOffset = 0;
            for (const auto& fv : s.mesh.num_face_vertices) {
                for (size_t v = 0; v < fv; ++v) {
                    tinyobj::index_t idx = s.mesh.indices[indexOffset + v];
                    I.push_back(indexOffset + v);
                    tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index];
                    tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                    tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
                    V.emplace_back(vx, vy, vz);

                    if (!attrib.normals.empty()) {
                        tinyobj::real_t nx = attrib.normals[3 * idx.normal_index];
                        tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
                        tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
                        N.emplace_back(nx, ny, nz);
                    }
                    if (!attrib.texcoords.empty()) {
                        tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index];
                        tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];
                        UV.emplace_back(tx, ty);
                    }
                }
                indexOffset += fv;
            }
        }

        if (!N.empty() && N.size() != V.size())
            LOG(LEVEL_ERROR, "Expected number of normals (%d) does not match number of vertices (%d)",
                N.size(), V.size());

        if (!UV.empty() && UV.size() != V.size())
            LOG(LEVEL_ERROR, "Expected number of texture coordinates (%d) does not match number of vertices (%d)",
                UV.size(), V.size());

        auto mesh = std::make_shared<Mesh>(Transform(), I, V, N, UV);

        auto prims = std::vector<std::unique_ptr<Primitive>>(mesh->m_NumTri);
        for (int i = 0; i < mesh->m_NumTri; ++i) {
            auto tri = std::make_shared<Triangle>(mesh, i);
            prims[i] = std::make_unique<Primitive>(tri);
        }

        auto accel = std::make_shared<BVH>(std::move(prims));
        auto lights = std::vector<std::shared_ptr<Light>>();
        // lights.push_back(std::make_shared<DistantLight>(rotateZ(degToRad(-90)), tkSpectrum(10.0f)));

        // Adjust for scale, we want to get the longest axis and make that 95% of the image
        auto bb = accel->worldBoundingBox();
        Vec3f diag = bb.diagonal();
        tkFloat fovy = 40.0f;
        tkFloat halfLength = diag.magnitude() / 0.95 / 2;
        tkFloat dist = halfLength / tan(degToRad(fovy / 2));

        Vec3f dir = normalize(cross(diag, Vec3f(0, 1, 0)) + Vec3f(0, diag.y / 8, 0));
        Point3f at = bb.center();
        Point3f eye = at + dir * dist;
        Transform cameraToWorld = lookAt(eye, at, Vec3f(0, 1, 0));
        g_Context.setImage(std::make_unique<PNGImage>(g_Context.m_Resolution, g_Context.m_Outfile));
        auto camera = std::make_shared<PerspectiveCamera>(cameraToWorld, 0.0f, (at - eye).magnitude(), fovy,
                                                          g_Context.image());

        // Round samplesPerPixel to nearest power of 2, then set to x/y, and get number of dimensions needed
        g_Context.setSampler(std::make_unique<StratifiedSampler>(8, 8, 0));

        auto integrator = std::make_shared<AOIntegrator>(20, 20);

        g_Context.setScene(std::make_unique<Scene>(accel, lights, camera, integrator));
    }

    void setupCornellBox();
    void RenderAPI::parse(const std::string& inputFile) {
        // TODO: Read required data from file
        if (g_Context.m_InputType == OBJ) {
            parseObj(inputFile);
            return;
        }

        setupCornellBox();
    }

    void setupCornellBox() {
        Point3f eye(278, 273, -800);
        Point3f at(278, 273, 100);
        Transform cameraToWorld = lookAt(eye, at, Vec3f(0, 1, 0));

        g_Context.setImage(std::make_unique<PNGImage>(g_Context.m_Resolution, g_Context.m_Outfile));
        auto camera = std::make_shared<PerspectiveCamera>(cameraToWorld, 40.0f, (at - eye).magnitude(), 40.0f,
                                                          g_Context.image());
        // Round samplesPerPixel to nearest power of 2, then set to x/y, and get number of dimensions needed
        g_Context.setSampler(std::make_unique<StratifiedSampler>(8, 8, 0));

        tkSpectrum whiteKd(tkSpectrum::fromRGB(Vec3f(0.75f, 0.75f, 0.75f)));
        auto diffuseWhite = std::make_shared<Diffuse>(whiteKd);
        tkSpectrum redKd(tkSpectrum::fromRGB(Vec3f(0.66f, 0.0f, 0.0f)));
        auto diffuseRed = std::make_shared<Diffuse>(redKd);
        tkSpectrum greenKd(tkSpectrum::fromRGB(Vec3f(0.15f, 0.476f, 0.0f)));
        auto diffuseGreen = std::make_shared<Diffuse>(greenKd);
        // tkSpectrum mirrorKd(tkSpectrum::fromRGB(Vec3f(0.9, 0.9, 0.9)));
        // auto mirror = std::make_shared<Mirror>(mirrorKd);

        std::vector<std::unique_ptr<Primitive>> prims;

        // Lights
        std::vector<std::shared_ptr<Light>> lights;
        // lights.push_back(std::make_shared<DirectionalLight>(rotateZ(degToRad(-90)), tkSpectrum(10.0f)));
        // lights.push_back(std::make_shared<PointLight>(
        //     translate(Vec3f(343, 540, 227)), tkSpectrum(100000)));
        // lights.push_back(std::make_shared<PointLight>(
        //     translate(Vec3f(213, 540, 332)), tkSpectrum(100000)));
        // Transform lightPos = translate(Vec3f(278, 548.8, 279.5));
        // auto sphereLight = std::make_shared<Sphere>(&lightPos, 65);
        // auto areaLight = std::make_shared<AreaLight>(Transform(), sphereLight, tkSpectrum(10.0f));
        // lights.push_back(areaLight);
        // prims.push_back(std::make_shared<Primitive>(sphereLight, diffuseWhite, areaLight));

        // Geometry
        Transform tf = translate(Vec3f::zero);
        std::vector<Point3f> V = {
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
            // Left wall 10-11
            Point3f(552.8, 0, 0), Point3f(549.6, 0, 559.2), Point3f(556, 548.8, 559.2),
            Point3f(556, 548.8, 0),
            // Short block 12-21
            Point3f(130, 165, 65), Point3f(82, 165, 225), Point3f(240, 165, 272), Point3f(290, 165, 114),

            Point3f(290, 0, 114), Point3f(290, 165, 114), Point3f(240, 165, 272), Point3f(240, 0, 272),

            Point3f(130, 0, 65), Point3f(130, 165, 65), Point3f(290, 165, 114), Point3f(290, 0, 114),

            Point3f(82, 0, 225), Point3f(82, 165, 225), Point3f(130, 165, 65), Point3f(130, 0, 65),

            Point3f(240, 0, 272), Point3f(240, 165, 272), Point3f(82, 165, 225), Point3f(82, 0, 225),
            // Tall Block 22-31
            Point3f(423, 330, 247), Point3f(265, 330, 296), Point3f(314, 330, 456), Point3f(472, 330, 406),

            Point3f(423, 0, 247), Point3f(423, 330, 247), Point3f(472, 330, 406), Point3f(472, 0, 406),

            Point3f(472, 0, 406), Point3f(472, 330, 406), Point3f(314, 330, 456), Point3f(314, 0, 456),

            Point3f(314, 0, 456), Point3f(314, 330, 456), Point3f(265, 330, 296), Point3f(265, 0, 296),

            Point3f(265, 0, 296), Point3f(265, 330, 296), Point3f(423, 330, 247), Point3f(423, 0, 247)
        };
        std::vector<int64_t> I = { 0,  1,  2,  0,  2,  3,  8,  4,  7,  8,  7,  11, 8,  9,  5,  8,  5,  4,  12,
                           13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23, 24, 25,
                           26, 24, 26, 27, 28, 29, 30, 28, 30, 31, 32, 33, 34, 32, 34, 35, 36, 37, 38,
                           36, 38, 39, 40, 41, 42, 40, 42, 43, 44, 45, 46, 44, 46, 47, 48, 49, 50, 48,
                           50, 51, 52, 53, 54, 52, 54, 55, 56, 57, 58, 56, 58, 59, 60, 61, 62, 60, 62,
                           63, 5,  9,  10, 5,  10, 6,  7,  6,  10, 7,  10, 11, 4,  5,  6,  4,  6,  7 };
        auto mesh = std::make_shared<Mesh>(tf, I, V, std::vector<Vec3f>(), std::vector<Point2f>());
        auto tri0 = std::make_shared<Triangle>(mesh, 0);
        auto tri1 = std::make_shared<Triangle>(mesh, 1);
        auto tri2 = std::make_shared<Triangle>(mesh, 2);
        auto tri3 = std::make_shared<Triangle>(mesh, 3);
        auto tri4 = std::make_shared<Triangle>(mesh, 4);
        auto tri5 = std::make_shared<Triangle>(mesh, 5);
        auto tri6 = std::make_shared<Triangle>(mesh, 6);
        auto tri7 = std::make_shared<Triangle>(mesh, 7);
        auto tri8 = std::make_shared<Triangle>(mesh, 8);
        auto tri9 = std::make_shared<Triangle>(mesh, 9);
        auto tri10 = std::make_shared<Triangle>(mesh, 10);
        auto tri11 = std::make_shared<Triangle>(mesh, 11);
        auto tri12 = std::make_shared<Triangle>(mesh, 12);
        auto tri13 = std::make_shared<Triangle>(mesh, 13);
        auto tri14 = std::make_shared<Triangle>(mesh, 14);
        auto tri15 = std::make_shared<Triangle>(mesh, 15);
        auto tri16 = std::make_shared<Triangle>(mesh, 16);
        auto tri17 = std::make_shared<Triangle>(mesh, 17);
        auto tri18 = std::make_shared<Triangle>(mesh, 18);
        auto tri19 = std::make_shared<Triangle>(mesh, 19);
        auto tri20 = std::make_shared<Triangle>(mesh, 20);
        auto tri21 = std::make_shared<Triangle>(mesh, 21);
        auto tri22 = std::make_shared<Triangle>(mesh, 22);
        auto tri23 = std::make_shared<Triangle>(mesh, 23);
        auto tri24 = std::make_shared<Triangle>(mesh, 24);
        auto tri25 = std::make_shared<Triangle>(mesh, 25);
        auto tri26 = std::make_shared<Triangle>(mesh, 26);
        auto tri27 = std::make_shared<Triangle>(mesh, 27);
        auto tri28 = std::make_shared<Triangle>(mesh, 28);
        auto tri29 = std::make_shared<Triangle>(mesh, 29);
        auto tri30 = std::make_shared<Triangle>(mesh, 30);
        auto tri31 = std::make_shared<Triangle>(mesh, 31);
        auto tri32 = std::make_shared<Triangle>(mesh, 32);
        auto tri33 = std::make_shared<Triangle>(mesh, 33);
        auto tri34 = std::make_shared<Triangle>(mesh, 34);
        auto tri35 = std::make_shared<Triangle>(mesh, 35);
        auto tri36 = std::make_shared<Triangle>(mesh, 36);
        auto tri37 = std::make_shared<Triangle>(mesh, 37);
        auto areaLight1 = std::make_shared<AreaLight>(tf, tri36, tkSpectrum(50.0f));
        auto areaLight2 = std::make_shared<AreaLight>(tf, tri37, tkSpectrum(50.0f));
        prims.push_back(std::make_unique<Primitive>(tri0, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri1, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri2, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri3, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri4, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri5, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri6, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri7, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri8, diffuseGreen));
        prims.push_back(std::make_unique<Primitive>(tri9, diffuseGreen));
        prims.push_back(std::make_unique<Primitive>(tri10, diffuseRed));
        prims.push_back(std::make_unique<Primitive>(tri11, diffuseRed));
        /* Start blocks */
        prims.push_back(std::make_unique<Primitive>(tri12, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri13, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri14, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri15, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri16, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri17, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri18, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri19, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri20, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri21, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri22, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri23, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri24, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri25, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri26, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri27, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri28, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri29, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri30, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri31, diffuseWhite));
        /* End blocks */
        prims.push_back(std::make_unique<Primitive>(tri32, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri33, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri34, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri35, diffuseWhite));
        prims.push_back(std::make_unique<Primitive>(tri36, diffuseWhite, areaLight1));
        prims.push_back(std::make_unique<Primitive>(tri37, diffuseWhite, areaLight2));

        /*Transform sphereTf = translate(Vec3f(185, 397, 353));
        auto mirrorSphere = std::make_shared<Sphere>(&sphereTf, 103);
        prims.push_back(std::make_shared<Primitive>(mirrorSphere, mirror));*/

        lights.push_back(areaLight1);
        lights.push_back(areaLight2);

        // Accel Structure
        // auto accel = std::make_shared<Iterator>(prims);
        auto accel = std::make_shared<BVH>(std::move(prims));

        // auto integrator = std::make_shared<WhittedIntegrator>(3);
        auto integrator = std::make_shared<PathTracingIntegrator>(20);
        // auto integrator = std::make_shared<AOIntegrator>(100, 40);

        // Scene
        g_Context.setScene(std::make_unique<Scene>(accel, lights, camera, integrator));
    }

    void RenderAPI::render() {
        int tileSize = g_Context.m_TileSize;
        Vec2i res = g_Context.m_Resolution;
        Vec2i numTiles((res.x + tileSize - 1) / tileSize, (res.y + tileSize - 1) / tileSize);
        Scene& scene = *g_Context.scene();

        scene.getIntegrator()->preprocess(scene);
        for (int y = 0; y < numTiles.y; ++y) {
            for (int x = 0; x < numTiles.x; ++x) {
                // Calculate tile bounds
                Point2i minPt(x * tileSize, y * tileSize);
                Point2i maxPt(std::min(minPt.x + tileSize, res.x), std::min(minPt.y + tileSize, res.y));
                SCHEDULE_TASK(RenderTask, scene, g_Context.getSamplerClone(), minPt, maxPt,
                              g_Context.m_SamplesPerPixel);
            }
        }

        if (g_Context.m_Gui) {
            // timeraster::setImageBuffer(g_Context.image());
            // timeraster::mainLoop();
        }
    }

    void RenderAPI::shutdown() {
        ThreadPool* pool = g_Context.threadpool();
        if (pool != nullptr)
            pool->shutdown();

        Image* image = g_Context.image();
        if (image != nullptr)
            image->write();
    }
}  // namespace TK
