#pragma once

#include <queue>
#include <mutex>

template <class T>
class TsQueue : public std::queue<T> {
public:
    T pop_front();

    template <class U>
    void push_back(U&& value);

private:
    std::mutex mutex;
};