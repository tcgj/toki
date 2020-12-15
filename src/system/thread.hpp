#pragma once

#include <queue>

#include "toki.hpp"
#include "task.hpp"

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
        bool m_Done;
        int m_TaskCount;
        std::mutex m_Mutex;
        std::condition_variable m_CondVar;
        std::unordered_map<int, std::unique_ptr<Task>> m_TaskMap;
        std::priority_queue<Task*, std::vector<Task*>, TaskComparator> m_TaskQueue;
    };

    class ThreadPool {
    public:
        ThreadPool(int threadCount, std::shared_ptr<Scheduler> scheduler, std::shared_ptr<Logger> logger);

        static int getThreadID();

        int getThreadCount() const;

        Logger* getLogger() const;

        void shutdown();

    private:
        void workerLoop(int threadId);

        std::vector<std::thread> m_Threads;
        std::shared_ptr<Scheduler> m_Scheduler;
        std::shared_ptr<Logger> m_Logger;

        static thread_local int s_ThreadId;
    };
} // namespace TK
