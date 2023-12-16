#pragma once

#include "func.hpp"
#include "data.hpp"

#include <functional>
#include <atomic>
#include <memory>
#include <future>
#include <mutex>
#include <queue>
#include <any>
#include <iostream>
#include <condition_variable>
#include <stdexcept>
#include <shared_mutex>
#include <any>
#include <thread>
#include <unordered_set>

class ThreadPool {
public:
    ThreadPool();

    template <class F, class... Args>
    auto AddTask(F&& f, Args&&... args)
        -> std::shared_ptr<std::future<typename std::invoke_result<F, Args...>::type>>;

    void ChangeWorkersAmount(size_t new_amount);
    void AddWorkers(size_t amount);
    void KillWorkers(size_t amount);
    void FinishTasks();

    bool IsInPool(std::thread::id id);

    ~ThreadPool();

private:
    void AddWorker();

    class Worker {
    public:
        Worker(ThreadPool& tp);

        void Work();

    private:
        ThreadPool& tp;
    };

private:
    std::atomic<bool> stop = false;

    std::atomic<int> expected_workers_amount = 0;
    std::atomic<int> workers_amount = 0;
    std::unordered_set<std::thread::id> worker_ids;

    std::queue<std::function<void()>> task_queue;
    std::condition_variable cv;
    std::mutex queue_mutex;
    std::mutex set_mutex;

    std::mutex pool_mutex;
};

template <class F, class... Args>
auto ThreadPool::AddTask(F&& f, Args&&... args) 
        -> std::shared_ptr<std::future<typename std::invoke_result<F, Args...>::type>> {
    using return_type = std::invoke_result<F, Args...>::type;

    if (stop)
        throw std::runtime_error("Adding task to a stopped thread pool");

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    auto future = std::make_shared<std::future<return_type>>(task->get_future());
    {
        std::unique_lock lock(queue_mutex);
        task_queue.push([task]{(*task)();});
        cv.notify_one();
    }
    return future;
}


