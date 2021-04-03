#pragma once
#include <chrono>

using namespace std::chrono;

class Timer
{
public:
	void start();
	void stop();
	float getMs();

private:
	steady_clock::time_point startTime;
	steady_clock::time_point endTime;
};

