
#include <Windows.h>
#include <timeapi.h>
#include "Timer.h"

using namespace std::chrono;

void Timer::push(steady_clock::time_point last)
{
	Markers.push_back(last);
	if (Markers.size() > MaxMarkers)
		Markers.pop_front();
}

void Timer::pop()
{
	if (Markers.size())
		Markers.pop_front();
}

//	Public

Timer::Timer(bool precise) :last{ std::chrono::steady_clock::now() }
{
	if (precise)
		timeBeginPeriod(1);
}

Timer::~Timer()
{
	timeEndPeriod(1);
}

void Timer::reset()
{
	last = high_resolution_clock::now();
	while (Markers.size())
		Markers.pop_front();
	push(last);
}

float Timer::mark()
{
	const auto old = last;
	last = steady_clock::now();
	push(last);
	return duration<float>(last - old).count();
}

float Timer::check()
{
	return duration<float>(steady_clock::now() - last).count();
}

float Timer::skip()
{
	const auto old = last;
	last = steady_clock::now();
	for (unsigned int i = 0; i < Markers.size(); i++)
		Markers[i] += last - old;
	return duration<float>(last - old).count();
}

float Timer::average()
{
	if(Markers.size()<2)
		return 0.0f;
	return duration<float>(Markers.back() - Markers.front()).count() / float(Markers.size() - 1);
}

int Timer::getSize()
{
	return (int)Markers.size();
}

void Timer::setMax(unsigned int max)
{
	MaxMarkers = max;
}
