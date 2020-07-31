#include "parallel.hpp"

#include "math/math.hpp"

namespace TK {
    class ParallelForJob;
    // Parallel global variables
    static std::vector<std::thread> threads;
    static bool shutdownThreads = false;
    static std::shared_ptr<ParallelForJob> jobList = nullptr;
    static std::mutex jobListMutex;
    static std::condition_variable jobListCond;

    // Per-thread variables
    thread_local tkInt threadIndex;

    tkUInt getNumCores() {
        return std::max(1u, std::thread::hardware_concurrency());
    }

    struct JobFrag {
        std::shared_ptr<ParallelForJob> job;
        tkI64 start;
        tkI64 end;
    };

    class ParallelForJob {
    public:
        ParallelForJob(tkI64 loopCount, tkInt batchSize,
                    const std::function<void(tkI64)> &func)
            : func1D(std::move(func)),
              iterCount(loopCount),
              batchSize(batchSize) {}
        ParallelForJob(const tkVec2i &loopCount, tkInt batchSize,
                    const std::function<void(tkVec2i)> &func)
            : func2D(std::move(func)),
              iterCount(loopCount.x * loopCount.y),
              batchSize(batchSize),
              xCount(loopCount.x) {}

        bool done() const {
            return nextIter >= iterCount && activeWorkers == 0;
        }

        std::function<void(tkI64)> func1D = nullptr;
        std::function<void(tkVec2i)> func2D = nullptr;
        tkI64 iterCount;
        tkInt xCount = -1;
        tkInt batchSize;
        tkI64 nextIter = 0;
        tkInt activeWorkers = 0;
        std::shared_ptr<ParallelForJob> nextJob = nullptr;
    };

    bool preprocessJob(JobFrag *jobFrag) {
        ParallelForJob &job = *jobList;
        if (job.nextIter == job.iterCount) {
            jobList = job.nextJob;
            jobListCond.notify_all();
            return false;
        }

        jobFrag->start = job.nextIter;
        job.nextIter = std::min(jobFrag->start + job.batchSize, job.iterCount);
        jobFrag->end = job.nextIter;
        jobFrag->job = jobList;
        return true;
    }

    // Job execution function
    void executeJobFrag(const JobFrag &jobFrag) {
        ParallelForJob &job = *(jobFrag.job);
        if (job.func1D != nullptr) {
            for (tkI64 i = jobFrag.start; i < jobFrag.end; ++i) {
                job.func1D(i);
            }
        } else {
            for (tkI64 i = jobFrag.start; i < jobFrag.end; ++i) {
                job.func2D(tkVec2i(i % job.xCount, i / job.xCount));
            }
        }
    }

    // Worker thread initialization function
    void workerStart(tkInt index) {
        threadIndex = index;
        std::unique_lock<std::mutex> lock(jobListMutex);

        JobFrag jobFrag;
        while (!shutdownThreads) {
            if (jobList == nullptr)
                jobListCond.wait(lock);
            else if (preprocessJob(&jobFrag)) {
                // Job can be worked on
                jobFrag.job->activeWorkers++;
                lock.unlock();
                executeJobFrag(jobFrag);
                lock.lock();
                jobFrag.job->activeWorkers--;
            }
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
        std::shared_ptr<ParallelForJob> job = std::make_shared<ParallelForJob>(loopCount, batchSize, func);
        {
            std::lock_guard<std::mutex> lock(jobListMutex);
            job->nextJob = jobList;
            jobList = job;
        }

        // Notify worker threads to execute job
        jobListCond.notify_all();

        // Wait for job
        while (!job->done());
    }

    void parallelFor2D(const tkVec2i &loopCount, tkInt batchSize,
                       std::function<void(tkVec2i)> func) {
        if (func == nullptr)
            return;

        // Running on single thread, use standard for loop
        if (threads.empty() || loopCount.x * loopCount.y  <= 1) {
            for (tkInt y = 0; y < loopCount.y; ++y) {
                for (tkInt x = 0; x < loopCount.x; ++x) {
                    func(tkVec2i(x, y));
                }
            }
            return;
        }

        // Add job to list
        std::shared_ptr<ParallelForJob> job = std::make_shared<ParallelForJob>(loopCount, batchSize, func);
        {
            std::lock_guard<std::mutex> lock(jobListMutex);
            job->nextJob = jobList;
            jobList = job;
        }

        // Notify worker threads to execute job
        jobListCond.notify_all();

        // Wait for job
        while (!job->done());
    }

    void initThreads(tkInt threadCount) {
        // Set number of threads based on system options
        tkInt tc = threadCount < 0 ? getNumCores() : threadCount;

        // Create threads (main thread excluded)
        for (tkInt i = 0; i < tc; ++i) {
            threads.push_back(std::thread(workerStart, i + 1));
        }

        // Set up main thread
        threadIndex = 0;
    }

    void cleanupThreads() {
        if (threads.empty())
            return;
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
        threads.erase(threads.begin(), threads.end());
        shutdownThreads = false;
    }
}  // namespace TK
