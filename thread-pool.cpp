#include <thread>

#include "thread-pool.hpp"
#include "func.hpp"

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

Data ThreadPool::AddTasks(std::vector<Func>&& functions) {
    if (stop)
        throw std::runtime_error("Adding task to a stopped thread pool");

    Data data(*this);
    for (auto& func : functions)
    {
        std::unique_lock lock(queue_mutex);
        task_queue.push(std::move(func.task));
        cv.notify_one();
        data.Add(func.future);
    }
    return data;
}

ThreadPool::~ThreadPool() {
    stop = true;
    
    cv.notify_all();
    while (!task_queue.empty()) {}
    ChangeWorkersAmount(0);
    while (!worker_ids.empty()) {}
}