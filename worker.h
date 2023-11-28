#pragma once

#include <iostream>
#include <functional>
#include <atomic>

#include "tsqueue.h"

class Worker {
public:
    Worker(TsQueue<std::function<void ()>>& task_queue, std::atomic<size_t>& workers_amount, 
            std::atomic<size_t>& expected_workers_amount, int index);

    void Work();

private:
    int index;
    TsQueue<std::function<void ()>>& task_queue;
    std::atomic<size_t>& workers_amount;
    std::atomic<size_t>& expected_workers_amount;
};