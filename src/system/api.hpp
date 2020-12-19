#pragma once

#include "toki.hpp"
#include "thread.hpp"
#include "logger.hpp"

namespace TK {
    class TOKIContext {
    public:
        Scheduler* scheduler() const {
            return m_Scheduler.get();
        }

        ThreadPool* threadpool() const {
            return m_ThreadPool.get();
        }

        Logger* logger() const {
            return m_Logger.get();
        }

        void setScheduler(std::shared_ptr<Scheduler> scheduler) {
            m_Scheduler = std::move(scheduler);
        }

        void setThreadPool(std::unique_ptr<ThreadPool> threadPool) {
            m_ThreadPool = std::move(threadPool);
        }

        void setLogger(std::unique_ptr<Logger> logger) {
            m_Logger = std::move(logger);
        }

    private:
        std::shared_ptr<Scheduler> m_Scheduler;
        std::unique_ptr<ThreadPool> m_ThreadPool;
        std::unique_ptr<Logger> m_Logger;
    };

    struct Options {
        std::string outFile;
        int threadCount = -1;
        bool fastRender = false;
    };

    class RenderAPI {
    public:
        static void tokiConfigure(const Options& options);

        static void tokiParse(std::string inputFile);

        static void tokiRun();

        static void tokiShutdown();

    private:
        // Options
        static std::string outFile;
        static int threadCount;
        static bool fastRender;

        // Render Settings
        static int tileSize;
        static int samplesPerPixel;
        static Vec2i resolution;
    };

    // Serializing API

    // Global variables
    extern std::unique_ptr<TOKIContext> g_Context;
}  // namespace TK
