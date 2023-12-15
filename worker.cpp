#include <chrono>
#include <thread>

#include "thread-pool.h"

ThreadPool::Worker::Worker(ThreadPool& tp) 
    : tp(tp) {}

void ThreadPool::Worker::Work() {
    for (;;) {
        if (--tp.workers_amount >= tp.expected_workers_amount) {
            {
                std::lock_guard<std::mutex> lg(tp.set_mutex);
                tp.worker_ids.erase(std::this_thread::get_id());
            }
            return;
        } else {
            ++tp.workers_amount;
        }

        std::function<void()> task;
        {
            std::unique_lock lock(tp.queue_mutex);
            tp.cv.wait(lock, [this]{ return tp.stop || !tp.task_queue.empty();});
            if (tp.stop && tp.task_queue.empty())
                continue;
            
            task = std::move(tp.task_queue.front());
            tp.task_queue.pop();
        }
        
        task();
    }
}