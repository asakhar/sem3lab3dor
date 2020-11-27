project(Queue LANGUAGES CXX)
cmake_minimum_required(VERSION 3.12)

add_executable(StaticMethods main.cpp)
target_compile_definitions(StaticMethods PRIVATE NOOPERATOR)

add_executable(DynamicEdition main.cpp)
target_compile_definitions(DynamicEdition PRIVATE DYNAMIC)

add_executable(StaticOperators main.cpp)
