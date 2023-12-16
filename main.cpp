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

long long foo(std::shared_ptr<std::vector<std::vector<int>>> matrix, std::shared_ptr<ThreadPool> tp, int x1, int y1, int x2, int y2) {
    long long a = 0;

    if (x2 - x1 > 3000 || y2 - y1 > 3000) {
        auto data = tp->AddTasks({{foo, matrix, tp, x1, y1, (x1 + x2) / 2, (y1 + y2) / 2},
                                        {foo, matrix, tp, (x1 + x2) / 2, (y1 + y2) / 2, x2, y2},
                                        {foo, matrix, tp, (x1 + x2) / 2, y1, x2, (y1 + y2) / 2},
                                        {foo, matrix, tp, x1, (y1 + y2) / 2, (x1 + x2) / 2, y2}});

        auto result = data.Get();
        a = std::any_cast<long long>(**result[0]) + std::any_cast<long long>(**result[1]) + 
            std::any_cast<long long>(**result[2]) + std::any_cast<long long>(**result[3]);
    } else {
        for (int i = y1; i < y2; ++i)
            for (int j = x1; j < x2; ++j)
                a += (*matrix)[i][j];
    }

    return a;
}

int main() {
    auto tp_ptr = std::make_shared<ThreadPool>();
    tp_ptr->ChangeWorkersAmount(20);

    int n = 10000, m = 10000;

    auto ptr = std::make_shared<std::vector<std::vector<int>>>(n, std::vector<int>(m, 1));
    {
        Timer timer;
        auto f = tp_ptr->AddTasks({{foo, ptr, tp_ptr, 0, 0, 10000, 10000}});
        f.Get();
    }

    {
        Timer timer;
        long long ans = 0;
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                ans += (*ptr)[i][j];
    }
}