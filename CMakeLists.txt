project(Queue LANGUAGES CXX)
cmake_minimum_required(VERSION 3.16)
add_executable(main main.cpp)
add_compile_definitions(DYNAMIC LIST_VERSION)
