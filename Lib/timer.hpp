#pragma once 

#include<iostream>
using namespace std;
#include <chrono>

class Timer
{
public:
    Timer();
    ~Timer();

private:
    chrono::time_point<std::chrono::high_resolution_clock> start_point;
};
