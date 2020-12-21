#pragma once

#include <queue>

#include "task.hpp"

#define SCHEDULE_TASK(type, ...)           \
    Scheduler* scheduler = g_Context.scheduler(); \
    if (scheduler != nullptr)                \
        scheduler->scheduleTask(std::make_unique<type>(__VA_ARGS__));

namespace TK {
    class TaskComparator {
    public:
        bool operator()(Task* t1, Task* t2) const {
            return t1->getPriority() < t2->getPriority();
        }
    };

    class Scheduler {
    public:
        void scheduleTask(std::unique_ptr<Task> task);

        void rescheduleTask(Task* task);

        Task* getNextTask();

        void markTaskComplete(Task* task);

        void done();

    private:
        bool m_Done = false;
        int m_TaskCount = 0;
        std::mutex m_Mutex;
        std::condition_variable m_CondVar;
        std::unordered_map<int, std::unique_ptr<Task>> m_TaskMap;
        std::priority_queue<Task*, std::vector<Task*>, TaskComparator> m_TaskQueue;
    };

    class ThreadPool {
    public:
        ThreadPool(int threadCount, std::shared_ptr<Scheduler> scheduler);

        static int getThreadId();

        int getThreadCount() const;

        void shutdown();

    private:
        void workerLoop(int threadId);

        std::vector<std::thread> m_Threads;
        std::shared_ptr<Scheduler> m_Scheduler;

        static thread_local int s_ThreadId;
    };
} // namespace TK
