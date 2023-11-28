#include <iostream>
#include <vector>
#include <functional>

#include "worker.h"
#include "thread-pool.h"

template <class R, class F, class... Args>
std::function<R ()> wrap(F f, Args&&... args) {
    return std::bind(f, std::forward<Args>(args)...);
}

int main() {
    ThreadPool tp(2);

    for(;;){}
}