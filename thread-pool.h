#pragma once

#include <functional>
#include <atomic>
#include <memory>

#include "tsqueue.h"

class ThreadPool {
public:
    ThreadPool(size_t workers_amount);

private:
    TsQueue<std::function<void ()>> task_queue;
    std::atomic<size_t> workers_amount;
    std::atomic<size_t> expected_workers_amount;
};