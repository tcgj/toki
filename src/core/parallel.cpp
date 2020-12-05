#include "parallel.hpp"

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
        thread_local int threadIndex;

        unsigned int getNumCores() {
            return std::max(1u, std::thread::hardware_concurrency());
        }

        struct WorkSplit {
            int64_t start;
            int64_t end;
        };

        class Job {
        public:
            Job(int64_t workSize, int batchSize, std::function<void(int64_t)> func)
                : func1D(std::move(func)), iterCount(workSize), batchSize(batchSize) {}
            Job(const Vec2i& workSize, int batchSize, std::function<void(Vec2i)> func)
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

            std::function<void(int64_t)> func1D = nullptr;
            std::function<void(Vec2i)> func2D = nullptr;
            int64_t iterCount;
            int iterX = -1;
            int batchSize;
            int64_t nextIter = 0;
            int activeWorkers = 0;
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
                for (int64_t i = split.start; i < split.end; ++i) {
                    job.func1D(i);
                }
            } else if (job.func2D != nullptr) {
                for (int64_t i = split.start; i < split.end; ++i) {
                    job.func2D(Vec2i(i % job.iterX, i / job.iterX));
                }
            }
        }

        // Worker thread initialization function
        void workerStart(int index) {
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
        void dispatch(int64_t workSize, int batchSize, std::function<void(int64_t)> func) {
            if (func == nullptr)
                return;

            // Running on single thread, use standard for loop
            if (threads.empty() || workSize < batchSize) {
                for (int i = 0; i < workSize; ++i) {
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

        void dispatch2D(const Vec2i& workSize, int batchSize, std::function<void(Vec2i)> func) {
            if (func == nullptr)
                return;

            // Running on single thread, use standard for loop
            if (threads.empty() || workSize.x * workSize.y <= batchSize) {
                for (int y = 0; y < workSize.y; ++y) {
                    for (int x = 0; x < workSize.x; ++x) {
                        func(Vec2i(x, y));
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

        void initThreads(int threadCount) {
            // Set number of threads based on system options
            int tc = threadCount < 0 ? getNumCores() : threadCount;

            // Create threads (main thread excluded)
            for (int i = 0; i < tc; ++i) {
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
