#pragma once

#include "Window.h"

class App {

private:
	Window window;
	Timer timer;

	float scale = 480.f;
public:
	App();

	int Run();
	void doFrame();
};