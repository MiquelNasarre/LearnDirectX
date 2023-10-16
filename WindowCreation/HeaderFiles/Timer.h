#pragma once

#include <iostream>
#include <chrono>

class Timer {
private:
	std::chrono::steady_clock::time_point last;

public:
	Timer() :last{ std::chrono::steady_clock::now() } {}
	float Mark();
	float Peek();
};