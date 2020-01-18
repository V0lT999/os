cmake_minimum_required(VERSION 3.15)
project(5_omp_clion)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set(SOURCE_FILES main.cpp)
add_executable(openmp_test_clion ${SOURCE_FILES})

add_executable(5_omp_clion main.cpp)