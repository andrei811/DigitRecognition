cmake_minimum_required(VERSION 3.0.0)
project(NeuralNetwork VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 14)

add_executable(Main main.cpp "Lib/timer.hpp" "Lib/timer.cpp")
target_include_directories(Main PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Lib")

add_compile_definitions("BASE_DIR=\"${CMAKE_SOURCE_DIR}\"")