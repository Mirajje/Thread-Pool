cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

add_library(thread_pool worker.cpp thread-pool.cpp data.cpp future.cpp)