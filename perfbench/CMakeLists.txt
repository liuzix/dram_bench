cmake_minimum_required(VERSION 3.2)
project(perfbench)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


add_executable(perfbench main.cpp)

target_link_libraries(perfbench Threads::Threads)