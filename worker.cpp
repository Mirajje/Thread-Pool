#include <chrono>
#include <thread>

#include "thread-pool.h"

ThreadPool::Worker::Worker(ThreadPool& tp) 
    : tp(tp) {}

void ThreadPool::Worker::Work() {
    using namespace std::chrono_literals;

    for (;;) {
        if (--tp.workers_amount >= tp.expected_workers_amount) {
            --tp.idle_workers_amount;
            return;
        }
        else {
            ++tp.workers_amount;
        }

        auto result = tp.task_queue.try_pop_front();

        if (!result) {
            std::this_thread::sleep_for(200ms);
            continue;
        }

        --tp.idle_workers_amount;
        std::invoke(result.value());
        ++tp.idle_workers_amount;
    }
}