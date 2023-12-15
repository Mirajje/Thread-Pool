#pragma once

#include "future.hpp"
#include "thread-pool.h"

#include <any>
#include <vector>
#include <future>

class Futures {
public:
    Futures(ThreadPool&);

    template <class T>
    void Add(const std::shared_ptr<std::future<T>>&);

    void Wait();

private:
    ThreadPool& tp;
    std::vector<std::shared_ptr<FutureBase>> futures;
};

template <class T>
void Futures::Add(const std::shared_ptr<std::future<T>>& future) {
    futures.emplace_back(std::make_shared<Future<T>>(future));
}