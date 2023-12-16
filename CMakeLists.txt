cmake_minimum_required(VERSION 3.19)

project(thread_pool)

set(CMAKE_CXX_STANDARD 20)

add_executable(thread_pool main.cpp worker.cpp thread-pool.cpp data.cpp future.cpp)