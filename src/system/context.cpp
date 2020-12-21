#include "context.hpp"

#include "thread.hpp"
#include "core/sampler.hpp"
#include "core/scene.hpp"

namespace TK {
    TOKIContext g_Context;

    Scheduler* TOKIContext::scheduler() const {
        return m_Scheduler.get();
    }

    ThreadPool* TOKIContext::threadpool() const {
        return m_ThreadPool.get();
    }

    Logger* TOKIContext::logger() const {
        return m_Logger.get();
    }

    Scene* TOKIContext::scene() const {
        return m_Scene.get();
    }

    Image* TOKIContext::image() const {
        return m_Image.get();
    }

    std::unique_ptr<Sampler> TOKIContext::getSamplerClone() const {
        return m_Sampler->getClone();
    }

    void TOKIContext::setScheduler(std::shared_ptr<Scheduler> scheduler) {
        m_Scheduler = std::move(scheduler);
    }

    void TOKIContext::setThreadPool(std::unique_ptr<ThreadPool> threadPool) {
        m_ThreadPool = std::move(threadPool);
    }

    void TOKIContext::setLogger(std::unique_ptr<Logger> logger) {
        m_Logger = std::move(logger);
    }

    void TOKIContext::setScene(std::unique_ptr<Scene> scene) {
        m_Scene = std::move(scene);
    }

    void TOKIContext::setSampler(std::unique_ptr<Sampler> sampler) {
        m_Sampler = std::move(sampler);
    }

    void TOKIContext::setImage(std::unique_ptr<Image> image) {
        m_Image = std::move(image);
    }
} // namespace TK
