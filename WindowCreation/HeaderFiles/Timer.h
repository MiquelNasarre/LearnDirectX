#pragma once

#include <iostream>
#include <chrono>
#include <queue>

class Timer {
private:
	unsigned int MaxMarkers = 60u;
	std::chrono::steady_clock::time_point last;
	std::deque<std::chrono::steady_clock::time_point> Markers;

	void push(std::chrono::steady_clock::time_point last);
	void pop();

public:
	Timer(bool precise = true);
	~Timer();
	void reset();
	float mark();
	float check();
	float skip();
	float average();
	int getSize();
	void setMax(unsigned int max);
};