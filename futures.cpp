#include "futures.hpp"

Futures::Futures(ThreadPool& tp)
    : tp(tp) {}

void Futures::Wait() {
    bool is_in_pool = tp.IsInPool(std::this_thread::get_id());

    if (is_in_pool) {
        tp.AddWorkers(1);
    }

    for (auto& future : futures)
        future->Wait();

    if (is_in_pool) {
        tp.KillWorkers(1);
    }   
}