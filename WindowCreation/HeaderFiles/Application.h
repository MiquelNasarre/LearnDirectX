#pragma once

#include "Window.h"

class App {

private:
	Window window;

public:
	App();

	int Run();
	void doFrame();
};