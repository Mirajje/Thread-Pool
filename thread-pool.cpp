#include <thread>

#include "thread-pool.h"
#include "worker.h"

ThreadPool::ThreadPool(size_t amount) 
    : expected_workers_amount(amount), workers_amount(0) {
    for (size_t i = 0; i < expected_workers_amount; ++i) {
        ++workers_amount;
        Worker worker(task_queue, workers_amount, expected_workers_amount, i);
        std::thread thrd(&Worker::Work, worker);
        thrd.detach();
    }
}