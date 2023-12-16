#pragma once

#include <iostream>
#include <future>
#include <functional>

class ThreadPool;

template <class F, class... Args>
class Func {
public:
    Func(F&&, Args&&...);

private:
    std::shared_ptr<std::packaged_task<typename std::invoke_result<F, Args...>::type>> task;

    friend class ThreadPool;
};

template <class F, class... Args>
Func<F, Args...>::Func(F&& f, Args&&... args) {
    using return_type = std::invoke_result<F, Args...>::type;

    task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
}