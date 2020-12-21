#pragma once

#include "toki.hpp"
#include "thread.hpp"
#include "logger.hpp"

namespace TK {
    struct Options {
        std::string outfile;
        int threadCount = 0;
        bool fastRender = false;
    };

    namespace RenderAPI {
        void configure(const Options& options);

        void parse(std::string inputFile);

        void run();

        void shutdown();
    }  // namespace RenderAPI

    // Serializing API

    // Context Object
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

        // Options
        std::string m_Outfile;
        int m_ThreadCount = 0;
        bool m_FastRender = false;

        // Render Settings
        int m_TileSize = 16;
        int m_SamplesPerPixel = 16;
        Vec2i m_Resolution = { 800, 800 };

    private:
        std::shared_ptr<Scheduler> m_Scheduler;
        std::unique_ptr<ThreadPool> m_ThreadPool;
        std::unique_ptr<Logger> m_Logger;
    };

    // Global context
    extern TOKIContext g_Context;
}  // namespace TK
