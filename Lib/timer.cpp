#include "timer.hpp"

Timer::Timer()
{
	start_point = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
    auto endTimepoint = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<chrono::milliseconds>(start_point).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<chrono::milliseconds>(endTimepoint).time_since_epoch().count();

    auto duration = end - start;

    cout << "\nThe execution took: " << duration << "ms\n";
}