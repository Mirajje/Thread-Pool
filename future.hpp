#pragma once

#include <future>
#include <any>

class FutureBase {
public:
    virtual void Wait() = 0;
    virtual bool Valid() = 0;
    virtual std::unique_ptr<std::any> Get() = 0;
    virtual ~FutureBase() = 0;
};

template <class T>
class Future : public FutureBase {
public:
    Future(const std::shared_ptr<std::future<T>>& future);

    void Wait() override;
    bool Valid() override;
    std::unique_ptr<std::any> Get() override;

private:
    std::shared_ptr<std::future<T>> future;
};

template <class T>
Future<T>::Future(const std::shared_ptr<std::future<T>>& future)
    : future(future) {}

template <class T>
void Future<T>::Wait() {
    future->wait();
}

template <class T>
bool Future<T>::Valid() {
    return future->valid();
}

template <class T>
std::unique_ptr<std::any> Future<T>::Get() {
    return std::make_unique<std::any>(future->get());
}