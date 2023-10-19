#pragma once

#include "Window.h"

class App {

private:
	Window window;
	Timer timer;

public:
	App();

	int Run();
	void doFrame();
};