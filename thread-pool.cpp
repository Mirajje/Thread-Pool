#include <thread>

#include "thread-pool.h"

ThreadPool::ThreadPool() = default;

void ThreadPool::ChangeWorkersAmount(size_t new_amount) {
    std::lock_guard lg(pool_mutex);

    size_t cur_amount = expected_workers_amount;
    expected_workers_amount = new_amount;
    for (size_t i = cur_amount; i < new_amount; ++i) {
        ++idle_workers_amount;
        ++workers_amount;
        Worker worker(*this);
        std::thread thrd(&Worker::Work, std::move(worker));
        thrd.detach();
    }
}

ThreadPool::~ThreadPool() {
    stop = true;
    
    cv.notify_all();
    while (!task_queue.empty()) {}
    expected_workers_amount = 0;
    while (idle_workers_amount != 0 && workers_amount != 0) {}
}