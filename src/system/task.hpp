#pragma once

#include "toki.hpp"
#include "core/aabb.hpp"

namespace TK {
    enum TaskStatus {
        TASK_COMPLETE,
        TASK_INCOMPLETE
    };

    class Task {
    public:
        Task(int priorityLevel = 0);

        virtual TaskStatus execute() = 0;

        int getPriority() const;

        int getTaskId() const;

    protected:
        int m_TaskId;
        int m_PriorityLevel;

    private:
        std::mutex m_CounterMutex;
        static int s_TaskCounter;
    };

    class RenderTask : public Task {
    public:
        RenderTask(const Scene& scene, const Vec2i& origin, const Vec2i& tileSize, int numSamples);

        TaskStatus execute() override;

    private:
        Vec2i m_Origin;
        Vec2i m_TileSize;
        const Scene& m_Scene;
    };
} // namespace TK
