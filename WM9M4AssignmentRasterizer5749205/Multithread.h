#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

#include "renderer.h"

class ThreadPool {
private:
    bool stopThreadPool = false;
    std::condition_variable cv;        // Condition Variable for Signaling Changes
    std::condition_variable cv2;       
    std::mutex mtx;                    // Mutex for Synchronization
    std::queue<std::function<void()>> jobQueue;
    std::vector<std::thread> slaves;   // Vector of threads (worker threads/slaves)
    unsigned int active_jobs = 0;
public:
    ThreadPool(size_t cpu = std::thread::hardware_concurrency()) {
        for (size_t i = 0; i < cpu; ++i) {
            slaves.emplace_back([this] {
                while (true) {
                    std::function<void()> job;
                    {
                        // Safe syncronization for shared data
                        std::unique_lock<std::mutex> lock(mtx);

                        // Wait until a job or thread pool termination
                        cv.wait(lock, [this]() -> bool { return stopThreadPool || !jobQueue.empty(); });
                        if (stopThreadPool && jobQueue.empty()) return;

                        // Obtain the next job
                        job = std::move(jobQueue.front());
                        jobQueue.pop();
                        active_jobs += 1;
                    }
                    job();
                    {
                        std::unique_lock<std::mutex> lock(mtx);
                        active_jobs -= 1;
                        if (jobQueue.empty() && active_jobs == 0) cv2.notify_one();
                    }
                }
            });
        }
    }

    ~ThreadPool() {
        // Safe update to stop thread pool
        {
            std::unique_lock<std::mutex> lock(mtx);
            stopThreadPool = true;
        }
        cv.notify_all();

        // Make sure the workers/slaves finished their job
        for (auto& slave : slaves)
            slave.join();
    }

    void enqueue(std::function<void()> job) {
        // Safe update to enqueue job to the job queue
        {
            std::unique_lock<std::mutex> lock(mtx);
            jobQueue.emplace(std::move(job));
        }
        cv.notify_one();
    }

    void wait() {
        // Safe update to make thread pool wait
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv2.wait(lock, [this]() { return jobQueue.empty() && active_jobs == 0; });
        }
    }
};