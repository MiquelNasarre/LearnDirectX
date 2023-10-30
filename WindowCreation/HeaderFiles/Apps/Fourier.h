#pragma once

#include "Window.h"

#include "Drawable/Surface.h"

class Fourier {

private:
	Window window;

public:
	Fourier();

	int Run();


	void eventManager();
	void doFrame();
};