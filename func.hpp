#pragma once

#include <future>
#include <functional>
#include "future.hpp"

class ThreadPool;

class Func {
public:
    template <class F, class... Args>
    Func(F&&, Args&&...);

private:
    std::function<void()> task;
    std::shared_ptr<FutureBase> future;

    friend class ThreadPool;
};

template <class F, class... Args>
Func::Func(F&& f, Args&&... args) {
    using return_type = typename std::invoke_result<F, Args...>::type;

    auto p_task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    future = std::make_shared<Future<return_type>>(
        std::make_shared<std::future<return_type>>(p_task->get_future()));
    
    task = std::function<void()>([p_task]{(*p_task)();});
}