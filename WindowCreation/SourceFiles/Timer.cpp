#include "Timer.h"

using namespace std::chrono;

float Timer::Mark()
{
	const auto old = last;
	last = high_resolution_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float Timer::Peek()
{
	return duration<float>(steady_clock::now() - last).count();
}
