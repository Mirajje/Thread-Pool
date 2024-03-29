#pragma once

#include "future.hpp"
#include "thread-pool.hpp"

#include <any>
#include <vector>
#include <optional>

class Data {
public:
    std::vector<std::unique_ptr<std::optional<std::any>>> Get();
    std::optional<std::vector<std::unique_ptr<std::optional<std::any>>>> TryGet();

private:
    Data(ThreadPool&);

    void Add(const std::shared_ptr<FutureBase>&);

    ThreadPool& tp;
    std::vector<std::shared_ptr<FutureBase>> futures;

    friend class ThreadPool;
};