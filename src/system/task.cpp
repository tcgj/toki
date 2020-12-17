#include "task.hpp"

#include "core/integrator.hpp"
#include "core/sampler.hpp"
#include "core/scene.hpp"

namespace TK
{
    std::mutex Task::s_CounterMutex;
    int Task::s_TaskCounter = 0;

    Task::Task(int priorityLevel) : m_PriorityLevel(priorityLevel) {
        std::lock_guard<std::mutex> lock(s_CounterMutex);
        m_TaskId = s_TaskCounter;
        s_TaskCounter++;
    }

    int Task::getPriority() const {
        return m_PriorityLevel;
    }

    int Task::getTaskId() const {
        return m_TaskId;
    }

    RenderTask::RenderTask(const Scene& scene, std::unique_ptr<Sampler> sampler, const Point2i& origin,
                           const Vec2i& tileSize, int numSamples)
        : Task(numSamples),
          m_Scene(scene),
          m_Sampler(std::move(sampler)),
          m_Origin(origin),
          m_TileSize(tileSize) {}

    TaskStatus RenderTask::execute() {
        Camera* camera = m_Scene.getCamera();
        Integrator* integrator = m_Scene.getIntegrator();

        for (int y = m_Origin.y; y < m_TileSize.y; ++y) {
            for (int x = m_Origin.x; x < m_TileSize.x; ++x) {
                Point2i pix(x, y);
                CameraSample sample = m_Sampler->getCameraSample(pix);

                Ray ray;
                camera->generateRay(sample, ray);
                tkSpectrum li = integrator->Li(m_Scene, ray, *m_Sampler, 0);
            }
        }

        if (m_PriorityLevel > 1) {
            m_PriorityLevel--;
            return TASK_INCOMPLETE;
        }

        return TASK_COMPLETE;
    }
}