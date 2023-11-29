#pragma once

#include <queue>
#include <mutex>
#include <optional>

template <class T>
class TsQueue : public std::queue<T>{
public:
    std::optional<T> try_pop_front();

    template <class U>
    void push_back(U&& value);

private:
    std::mutex mutex;
};

template <class T>
std::optional<T> TsQueue<T>::try_pop_front() {
    std::lock_guard<std::mutex> lg(mutex);

    if (std::queue<T>::empty())
        return {};

    T value = std::move(std::queue<T>::front());
    std::queue<T>::pop();
    return value;
}

template <class T>
template <class U>
void TsQueue<T>::push_back(U&& value) {
    std::lock_guard<std::mutex> lg(mutex);

    std::queue<T>::push(std::forward<U>(value));
}