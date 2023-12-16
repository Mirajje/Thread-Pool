#include "future.hpp"

Future<void>::Future(const std::shared_ptr<std::future<void>>& future)
    : future(future) {}

void Future<void>::Wait() {
    future->wait();
}

bool Future<void>::Valid() {
    return future->valid();
}

std::unique_ptr<std::optional<std::any>> Future<void>::Get() {
    return std::make_unique<std::optional<std::any>>();
}