#pragma once

#include "Window.h"
#include "Drawable/Triangle.h"

class App {

private:
	Window window;
	Timer timer;

	Triangle test;

	float scale = 280.f;
	float theta = 3.14159f / 2.f;
	float phi = 0.f;
public:
	App();

	int Run();
	void doFrame();
};