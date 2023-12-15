#include <thread>

#include "thread-pool.h"

ThreadPool::ThreadPool() = default;

void ThreadPool::ChangeWorkersAmount(size_t new_amount) {
    std::lock_guard lg(pool_mutex);

    size_t cur_amount = expected_workers_amount;
    expected_workers_amount = new_amount;
    for (size_t i = cur_amount; i < new_amount; ++i) {
        AddWorker();
    }
}

void ThreadPool::AddWorkers(size_t amount) {
    std::lock_guard lg(pool_mutex);
    expected_workers_amount += amount;
    for (size_t i = 0; i < amount; ++i)
        AddWorker();
}

void ThreadPool::KillWorkers(size_t amount) {
    std::lock_guard lg(pool_mutex);
    expected_workers_amount -= amount;
}

void ThreadPool::AddWorker() {
    ++workers_amount;
    Worker worker(*this);
    std::thread thrd(&Worker::Work, std::move(worker));
    {
        std::lock_guard<std::mutex> lg(set_mutex);
        worker_ids.insert(thrd.get_id());
    }
    thrd.detach();
}

bool ThreadPool::IsInPool(std::thread::id id) {
    return worker_ids.count(id);
}

ThreadPool::~ThreadPool() {
    stop = true;
    
    cv.notify_all();
    while (!task_queue.empty()) {}
    ChangeWorkersAmount(0);
    while (!worker_ids.empty()) {}
}