#pragma once

#include "future.hpp"
#include "thread-pool.hpp"

#include <any>
#include <vector>
#include <future>
#include <optional>

class Data {
public:
    std::vector<std::unique_ptr<std::any>> Get();
    std::optional<std::vector<std::unique_ptr<std::any>>> TryGet();

private:
    Data(ThreadPool&);

    template <class T>
    void Add(const std::shared_ptr<std::future<T>>&);

    ThreadPool& tp;
    std::vector<std::shared_ptr<FutureBase>> futures;

    friend class ThreadPool;
};

template <class T>
void Data::Add(const std::shared_ptr<std::future<T>>& future) {
    futures.emplace_back(std::make_shared<Future<T>>(future));
}