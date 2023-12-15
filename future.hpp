#pragma once

#include <future>

class FutureBase {
public:
    virtual void Wait() = 0;
    virtual ~FutureBase() = default;
};

template <class T>
class Future : public FutureBase {
public:
    Future(const std::shared_ptr<std::future<T>>& future);

    void Wait() override;

private:
    std::shared_ptr<std::future<T>> future;
};

template <class T>
void Future<T>::Wait() {
    future->wait();
}

template <class T>
Future<T>::Future(const std::shared_ptr<std::future<T>>& future)
    : future(future) {}