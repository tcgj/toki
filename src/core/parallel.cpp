#include "parallel.hpp"

#include <thread>
#include <mutex>

#include "math/math.hpp"

namespace TK {
    class ParallelForJob;
    // Parallel global variables
    static std::vector<std::thread> threads;
    static bool shutdownThreads = false;
    static ParallelForJob *jobList = nullptr;
    static std::mutex jobListMutex;
    static std::condition_variable jobListCond;

    // Per-thread variables
    thread_local tkInt threadIndex;

    tkUInt getNumCores() {
        return std::max(1u, std::thread::hardware_concurrency());
    }

    class ParallelForJob {
    public:
        ParallelForJob(tkI64 loopCount, tkInt batchSize,
                    const std::function<void(tkI64)> &func)
            : func1D(std::move(func)),
              iterCount(loopCount),
              batchSize(batchSize) {}
        ParallelForJob(const tkVec2i &loopCount,
                    const std::function<void(tkVec2i)> &func)
            : func2D(std::move(func)),
              iterCount(loopCount.x * loopCount.y),
              batchSize(1),
              xCount(loopCount.x) {}

        bool done() const {
            return nextIter >= iterCount && activeWorkers == 0;
        }

        std::function<void(tkI64)> func1D;
        std::function<void(tkVec2i)> func2D;
        tkI64 iterCount;
        tkInt xCount = -1;
        tkInt batchSize;
        tkI64 nextIter = 0;
        tkInt activeWorkers = 0;
        ParallelForJob *nextJob = nullptr;
    };

    // Job execution function
    void executeJob(ParallelForJob &job) {
        std::unique_lock<std::mutex> lock(jobListMutex);
        while (!job.done()) {
            job.activeWorkers++;

            tkI64 start = job.nextIter;
            job.nextIter = std::min(start + job.batchSize, job.iterCount);
            if (job.nextIter == job.iterCount)
                jobList = job.nextJob;

            // Start on job and allow other threads to execute as well
            lock.unlock();
            for (tkI64 i = start; i < job.nextIter; ++i) {
                if (job.func1D != nullptr)
                    job.func1D(i);
                else
                    job.func2D(tkVec2i(i % job.xCount, i / job.xCount));
            }
            lock.lock();

            job.activeWorkers--;
        }
    }

    // Worker thread initialization function
    void workerStart(tkInt index) {
        threadIndex = index;
        std::unique_lock<std::mutex> lock(jobListMutex);
        while (!shutdownThreads) {
            if (jobList == nullptr)
                jobListCond.wait(lock);

            ParallelForJob &job = *jobList;
            executeJob(job);
            jobListCond.notify_all();
        }
    }

    // Parallel for loop initialization function
    void parallelFor(tkI64 loopCount, tkInt batchSize,
                     std::function<void(tkI64)> func) {
        if (func == nullptr)
            return;

        // Running on single thread, use standard for loop
        if (threads.empty() || loopCount < batchSize) {
            for (tkInt i = 0; i < loopCount; ++i) {
                func(i);
            }
            return;
        }

        // Add job to list
        ParallelForJob job(loopCount, batchSize, func);
        {
            std::lock_guard<std::mutex> lock(jobListMutex);
            job.nextJob = jobList;
            jobList = &job;
        }

        // Notify worker threads to execute job
        std::unique_lock<std::mutex> lock(jobListMutex);
        jobListCond.notify_all();
        // Main thread starts with execution
        executeJob(job);
    }

    void parallelFor2D(const tkVec2i &loopCount, std::function<void(tkVec2i)> func) {
        if (func == nullptr)
            return;

        // Running on single thread, use standard for loop
        if (threads.empty() || loopCount.x * loopCount.y  <= 1) {
            for (tkInt y = 0; y < loopCount.y; ++y) {
                for (tkInt x = 0; x < loopCount.x; ++y) {
                    func(tkVec2i(x, y));
                }
            }
            return;
        }

        // Add job to list
        ParallelForJob job(loopCount, func);
        {
            std::lock_guard<std::mutex> lock(jobListMutex);
            job.nextJob = jobList;
            jobList = &job;
        }

        // Notify worker threads to execute job
        std::unique_lock<std::mutex> lock(jobListMutex);
        jobListCond.notify_all();
        // Main thread starts with execution
        executeJob(job);
    }

    void initThreads() {
        // Set number of threads based on system options
        tkInt threadCount = getNumCores();

        // Create threads (main thread included)
        for (tkInt i = 0; i < threadCount - 1; ++i) {
            threads.push_back(std::thread(workerStart, i + 1));
        }

        // Set up main thread
        threadIndex = 0;
    }

    void cleanupThreads() {
        if (threads.empty()) return;

        // Signal thread shutdown
        {
            std::lock_guard<std::mutex> lock(jobListMutex);
            shutdownThreads = true;
            jobListCond.notify_all();
        }
        for (auto &thread : threads) {
            thread.join();
        }

        // Reset variables
        threads.clear();
        shutdownThreads = false;
    }
}  // namespace TK
