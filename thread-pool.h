#pragma once

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

class ThreadPool {
public:
    ThreadPool();

    template <class F, class... Args>
    auto AddTask(F&& f, Args&&... args)
        -> std::future<typename std::invoke_result<F, Args...>::type>;

    void ChangeWorkersAmount(size_t new_amount);
    void FinishTasks();

    ~ThreadPool();

private:
    class Worker {
    public:
        Worker(ThreadPool& tp);

        void Work();

    private:
        ThreadPool& tp;
    };

    std::atomic<int> expected_workers_amount = 0;
    std::atomic<int> workers_amount = 0;
    std::atomic<int> idle_workers_amount = 0;

    std::atomic<bool> stop = false;
    std::queue<std::function<void()>> task_queue;
    std::condition_variable cv;
    std::mutex queue_mutex;

    std::mutex pool_mutex;
};

template <class F, class... Args>
auto ThreadPool::AddTask(F&& f, Args&&... args) 
        -> std::future<typename std::invoke_result<F, Args...>::type> {
    using return_type = std::invoke_result<F, Args...>::type;

    if (stop)
        throw std::runtime_error("Adding task to a stopped thread pool");

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<return_type> future = task->get_future();
    {
        std::unique_lock lock(queue_mutex);
        task_queue.push([task]{(*task)();});
        cv.notify_one();
    }
    return future;
}