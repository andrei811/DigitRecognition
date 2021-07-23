cmake_minimum_required(VERSION 3.0.0)
project(NeuralNetwork VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)

# add_subdirectory("Lib")

add_executable(Main main.cpp)
# target_link_libraries (Main LINK_PUBLIC Matrix)
target_include_directories(Main PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Lib")
# target_link_libraries(Main Matrix)