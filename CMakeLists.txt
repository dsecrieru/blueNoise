cmake_minimum_required(VERSION 3.25)

project(blueNoise)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(blueNoise main.cpp)
add_executable(generator generator.cpp generator.cpp)