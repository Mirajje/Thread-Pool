#include "tsqueue.h"

template <class T>
T TsQueue<T>::pop_front() {
    std::lock_guard<std::mutex> lg(mutex);

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