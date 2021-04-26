#include "Timer.h"

using namespace std;
using namespace std::chrono;

void Timer::start()
{
	this->startTime = high_resolution_clock::now();
}

void Timer::stop()
{
	this->endTime = high_resolution_clock::now();
}

float Timer::getMs()
{
	duration<float, milli> milliseconds = this->endTime - this->startTime;
	float msCount = milliseconds.count();
		
	return msCount;
}
