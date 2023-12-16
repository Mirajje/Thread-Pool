#include "data.hpp"
#include <memory>
#include <optional>

Data::Data(ThreadPool& tp)
    : tp(tp) {}

void Data::Add(const std::shared_ptr<FutureBase>& future) {
    futures.emplace_back(future);
}

std::vector<std::unique_ptr<std::optional<std::any>>> Data::Get() {
    bool is_in_pool = tp.IsInPool(std::this_thread::get_id());

    if (is_in_pool) {
        tp.AddWorkers(1);
    }

    for (auto& future : futures)
        future->Wait();

    if (is_in_pool) {
        tp.KillWorkers(1);
    }

    std::vector<std::unique_ptr<std::optional<std::any>>> result;
    result.reserve(futures.size());
    for (auto& future : futures) {
        result.push_back(future->Get());
    }

    return result;
}

std::optional<std::vector<std::unique_ptr<std::optional<std::any>>>> Data::TryGet() {
    for (auto& future : futures)
        if (!future->Valid())
            return {};

    std::vector<std::unique_ptr<std::optional<std::any>>> result;
    result.reserve(futures.size());
    for (auto& future : futures) {
        result.push_back(future->Get());
    }

    return result;
}