#pragma once

#include "toki.hpp"

namespace TK {
    class TOKIContext {
    public:
        Scheduler* scheduler() const;

        ThreadPool* threadpool() const;

        Logger* logger() const;

        Scene* scene() const;

        Image* image() const;

        std::unique_ptr<Sampler> getSamplerClone() const;

        void setScheduler(std::shared_ptr<Scheduler> scheduler);

        void setThreadPool(std::unique_ptr<ThreadPool> threadPool);

        void setLogger(std::unique_ptr<Logger> logger);

        void setScene(std::unique_ptr<Scene> scene);

        void setSampler(std::unique_ptr<Sampler> sampler);

        void setImage(std::unique_ptr<Image> image);

        // System Options
        std::string m_Outfile;
        int m_ThreadCount = 0;
        bool m_FastRender = false;

        // Render Options
        int m_TileSize = 16;
        int m_SamplesPerPixel = 8;
        Vec2i m_Resolution = { 400, 400 };

    private:
        // System Context
        std::shared_ptr<Scheduler> m_Scheduler;
        std::unique_ptr<ThreadPool> m_ThreadPool;
        std::unique_ptr<Logger> m_Logger;

        // Render Context
        std::unique_ptr<Scene> m_Scene;
        std::unique_ptr<Sampler> m_Sampler;
        std::unique_ptr<Image> m_Image;
    };

    // Global context
    extern TOKIContext g_Context;
} // namespace TK
