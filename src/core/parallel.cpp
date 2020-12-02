#include "parallel.hpp"

#include "math/math.hpp"

namespace TK {
    namespace Parallel {
        class Job;
        // Parallel global variables
        static std::vector<std::thread> threads;
        static bool shutdownThreads = false;
        static Job* jobQueue = nullptr;
        static std::mutex jobQueueMutex;
        static std::condition_variable jobQueueCond;

        // Per-thread variables
        thread_local tkInt threadIndex;

        tkUInt getNumCores() {
            return std::max(1u, std::thread::hardware_concurrency());
        }

        struct WorkSplit {
            tkI64 start;
            tkI64 end;
        };

        class Job {
        public:
            Job(tkI64 workSize, tkInt batchSize, std::function<void(tkI64)> func)
                : func1D(std::move(func)), iterCount(workSize), batchSize(batchSize) {}
            Job(const tkVec2i& workSize, tkInt batchSize, std::function<void(tkVec2i)> func)
                : func2D(std::move(func)),
                  iterCount(workSize.x * workSize.y),
                  batchSize(batchSize),
                  iterX(workSize.x) {}

            bool hasWork() const {
                return nextIter < iterCount;
            }
            bool done() const {
                std::unique_lock<std::mutex> lock(jobQueueMutex);
                return !hasWork() && activeWorkers == 0;
            }

            std::function<void(tkI64)> func1D = nullptr;
            std::function<void(tkVec2i)> func2D = nullptr;
            tkI64 iterCount;
            tkInt iterX = -1;
            tkInt batchSize;
            tkI64 nextIter = 0;
            tkInt activeWorkers = 0;
            Job* nextJob = nullptr;
        };

        WorkSplit getWork(Job& job) {
            WorkSplit ret;
            ret.start = job.nextIter;
            job.nextIter = std::min(ret.start + job.batchSize, job.iterCount);
            ret.end = job.nextIter;

            return ret;
        }

        // Job execution function
        void executeJob(const Job& job, const WorkSplit& split) {
            if (job.func1D != nullptr) {
                for (tkI64 i = split.start; i < split.end; ++i) {
                    job.func1D(i);
                }
            } else if (job.func2D != nullptr) {
                for (tkI64 i = split.start; i < split.end; ++i) {
                    job.func2D(tkVec2i(i % job.iterX, i / job.iterX));
                }
            }
        }

        // Worker thread initialization function
        void workerStart(tkInt index) {
            threadIndex = index;
            std::unique_lock<std::mutex> lock(jobQueueMutex);

            while (!shutdownThreads) {
                if (jobQueue == nullptr)
                    jobQueueCond.wait(lock);
                else {
                    Job& job = *jobQueue;
                    WorkSplit split = getWork(job);
                    if (!job.hasWork()) {
                        jobQueue = job.nextJob;
                        jobQueueCond.notify_all();
                    }

                    job.activeWorkers++;
                    lock.unlock();
                    executeJob(job, split);
                    lock.lock();
                    job.activeWorkers--;
                }
            }
        }

        // Parallel for loop initialization function
        void dispatch(tkI64 workSize, tkInt batchSize, std::function<void(tkI64)> func) {
            if (func == nullptr)
                return;

            // Running on single thread, use standard for loop
            if (threads.empty() || workSize < batchSize) {
                for (tkInt i = 0; i < workSize; ++i) {
                    func(i);
                }
                return;
            }

            // Add job to list
            Job* job = new Job(workSize, batchSize, func);
            {
                std::lock_guard<std::mutex> lock(jobQueueMutex);
                job->nextJob = jobQueue;
                jobQueue = job;
            }

            // Notify worker threads to execute job
            jobQueueCond.notify_all();

            // Wait for job
            while (!job->done())
                ;
        }

        void dispatch2D(const tkVec2i& workSize, tkInt batchSize, std::function<void(tkVec2i)> func) {
            if (func == nullptr)
                return;

            // Running on single thread, use standard for loop
            if (threads.empty() || workSize.x * workSize.y <= batchSize) {
                for (tkInt y = 0; y < workSize.y; ++y) {
                    for (tkInt x = 0; x < workSize.x; ++x) {
                        func(tkVec2i(x, y));
                    }
                }
                return;
            }

            // Add job to list
            Job* job = new Job(workSize, batchSize, func);
            {
                std::lock_guard<std::mutex> lock(jobQueueMutex);
                job->nextJob = jobQueue;
                jobQueue = job;
            }

            // Notify worker threads to execute job
            jobQueueCond.notify_all();

            // Wait for job
            while (!job->done())
                ;
            delete job;
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
                std::lock_guard<std::mutex> lock(jobQueueMutex);
                shutdownThreads = true;
                jobQueueCond.notify_all();
            }

            for (auto& thread : threads) {
                thread.join();
            }

            // Reset variables
            threads.erase(threads.begin(), threads.end());
            shutdownThreads = false;
        }
    }  // namespace Parallel
}  // namespace TK
