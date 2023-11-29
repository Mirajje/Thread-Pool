#include <thread>

#include "thread-pool.h"

ThreadPool::ThreadPool() = default;

void ThreadPool::ChangeWorkersAmount(size_t new_amount) {
    expected_workers_amount = new_amount;
    for (size_t i = workers_amount; i < new_amount; ++i) {
        ++idle_workers_amount;
        ++workers_amount;
        Worker worker(*this);
        std::thread thrd(&Worker::Work, std::move(worker));
        thrd.detach();
    }
}

void ThreadPool::FinishTasks() {
    while (!task_queue.empty()) {}
    while (idle_workers_amount < expected_workers_amount) {}
}

ThreadPool::~ThreadPool() {
    FinishTasks();
    
    expected_workers_amount = 0;
    while (idle_workers_amount) {}
}