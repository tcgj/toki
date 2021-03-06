#include "thread.hpp"

#include "task.hpp"

namespace TK {
    thread_local int ThreadPool::s_ThreadId;

    ThreadPool::ThreadPool(int threadCount, std::shared_ptr<Scheduler> scheduler)
        : m_Scheduler(std::move(scheduler)) {
        int tc = threadCount < 0 ? std::max(1u, std::thread::hardware_concurrency()) : threadCount;

        s_ThreadId = 0;
        m_Threads.reserve(tc);
        for (int i = 0; i < tc; ++i) {
            m_Threads.emplace_back(&ThreadPool::workerLoop, this, i + 1);
        }
    }

    int ThreadPool::getThreadId() {
        return s_ThreadId;
    }

    int ThreadPool::getThreadCount() const {
        return m_Threads.size();
    }

    void ThreadPool::shutdown() {
        if (m_Threads.empty())
            return;

        m_Scheduler->done();

        for (auto& t : m_Threads) {
            t.join();
        }
        m_Threads.clear();
    }

    void ThreadPool::workerLoop(int threadId) {
        s_ThreadId = threadId;

        Task* currentTask;
        while (true) {
            currentTask = m_Scheduler->getNextTask();

            if (currentTask == nullptr)
                return;

            switch (currentTask->execute()) {
                case TASK_INCOMPLETE:
                    m_Scheduler->rescheduleTask(currentTask);
                    break;
                default:
                    m_Scheduler->markTaskComplete(currentTask);
                    break;
            }
        }
    }

    void Scheduler::scheduleTask(std::unique_ptr<Task> task) {
        std::lock_guard<std::mutex> lock(m_Mutex);

        int id = task->getTaskId();
        m_TaskMap[id] = std::move(task);
        m_TaskCount++;
        m_TaskQueue.push(m_TaskMap[id].get());
        m_CondVar.notify_one();
    }

    void Scheduler::rescheduleTask(Task* task) {
        std::lock_guard<std::mutex> lock(m_Mutex);

        m_TaskQueue.push(task);
        m_CondVar.notify_one();
    }

    Task* Scheduler::getNextTask() {
        std::unique_lock<std::mutex> lock(m_Mutex);

        if (m_TaskQueue.size() == 0)
            m_CondVar.wait(lock);

        if (m_Done)
            return nullptr;

        Task* task = m_TaskQueue.top();
        m_TaskQueue.pop();
        return task;
    }

    void Scheduler::markTaskComplete(Task* task) {
        std::lock_guard<std::mutex> lock(m_Mutex);

        m_TaskMap.erase(task->getTaskId());
        m_TaskCount--;
    }

    void Scheduler::done() {
        while (m_TaskCount > 0) {}
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Done = true;
        }
        m_CondVar.notify_all();
    }
}  // namespace TK