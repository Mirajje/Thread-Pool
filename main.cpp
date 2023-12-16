#include "thread-pool.hpp"

#include <iostream>
#include <functional>
#include <chrono>

struct Timer {
    Timer() {
        start = std::chrono::system_clock::now();
    }

    ~Timer() {
        auto duration = std::chrono::duration<double>(std::chrono::system_clock::now() - start).count();
        std::cout << duration << "\n";
    }

    std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> start;
};

// long long foo(std::shared_ptr<std::vector<std::vector<int>>> matrix, std::shared_ptr<ThreadPool> tp, int x1, int y1, int x2, int y2) {
//     long long a = 0;

//     if (x2 - x1 > 3000 || y2 - y1 > 3000) {
//         Futures futures(*tp);

//         auto f1 = tp->AddTask(foo, matrix, tp, x1, y1, (x1 + x2) / 2, (y1 + y2) / 2);
//         futures.Add(f1);
//         auto f2 = tp->AddTask(foo, matrix, tp, (x1 + x2) / 2, (y1 + y2) / 2, x2, y2);
//         futures.Add(f2);
//         auto f3 = tp->AddTask(foo, matrix, tp, (x1 + x2) / 2, y1, x2, (y1 + y2) / 2);
//         futures.Add(f3);
//         auto f4 = tp->AddTask(foo, matrix, tp, x1, (y1 + y2) / 2, (x1 + x2) / 2, y2);
//         futures.Add(f4);

//         futures.Wait();
//         a = f1->get() + f2->get() + f3->get() + f4->get();
//     } else {
//         for (int i = y1; i < y2; ++i)
//             for (int j = x1; j < x2; ++j)
//                 a += (*matrix)[i][j];
//     }

//     return a;
// }

int main() {
    // auto tp_ptr = std::make_shared<ThreadPool>();
    // tp_ptr->ChangeWorkersAmount(20);
    // int n = 10000, m = 10000;

    // auto ptr = std::make_shared<std::vector<std::vector<int>>>(n, std::vector<int>(m, 1));

    // {
    //     Timer timer;
    //     auto f = tp_ptr->AddTask(foo, ptr, tp_ptr, 0, 0, 10000, 10000);
    // }

    // {
    //     Timer timer;
    //     long long ans = 0;
    //     for (int i = 0; i < n; ++i)
    //         for (int j = 0; j < m; ++j)
    //             ans += (*ptr)[i][j];
    // }
}