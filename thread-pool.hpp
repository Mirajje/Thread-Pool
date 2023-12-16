#pragma once

#include "func.hpp"
#include "data.hpp"

#include <unordered_set>
#include <queue>

class Data;

class ThreadPool {
public:
    ThreadPool();

    Data AddTasks(std::vector<Func>&&);

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

