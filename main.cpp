#include <iostream>

#include "thread-pool.h"

int main() {
    ThreadPool tp;
    tp.ChangeWorkersAmount(5);

    std::atomic<long long> a = 0;
    for (int i = 0; i < 5; ++i)
        tp.AddTask([&a] {
            for (long long i = 0; i < 1000000000; ++i)
                ++a;
        });

    tp.FinishTasks();
}