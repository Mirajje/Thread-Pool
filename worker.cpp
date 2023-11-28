#include "worker.h"

Worker::Worker(TsQueue<std::function<void ()>>& task_queue, std::atomic<size_t>& workers_amount, 
        std::atomic<size_t>& expected_workers_amount, int index) 
    : task_queue(task_queue), workers_amount(workers_amount), expected_workers_amount(expected_workers_amount), index(index) {}

void Worker::Work() {
    for (;;) {
        std::cout << index << "\n";
    }
}