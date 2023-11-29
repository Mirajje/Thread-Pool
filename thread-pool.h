#pragma once

#include <functional>
#include <atomic>
#include <memory>
#include <future>

#include "tsqueue.h"

class ThreadPool {
public:
    ThreadPool();

    template <class F, class... Args>
    void AddTask(F f, Args&&... args);

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

    TsQueue<std::function<void ()>> task_queue;
    std::atomic<size_t> workers_amount = 0;
    std::atomic<size_t> idle_workers_amount = 0;
    std::atomic<size_t> expected_workers_amount = 0;
};

template <class F, class... Args>
void ThreadPool::AddTask(F f, Args&&... args) {
    std::function<void ()> functor = std::bind(f, std::forward<Args>(args)...);
    task_queue.push_back(std::move(functor));    
}