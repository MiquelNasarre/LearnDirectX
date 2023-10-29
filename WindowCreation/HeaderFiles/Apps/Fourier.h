#pragma once

#include "Window.h"

#include "Drawable/Surface.h"

class Fourier {

private:
	Window window;

	Surface surf;

public:
	Fourier();

	int Run();


	void eventManager();
	void doFrame();
};

//	Surface functions

float SincFunction(float, float);

float constantRadius(float, float);

float constantRadius03(float, float);

float weirdRadius(float, float);

float returnX(float, float);
 
float returnY(float, float);

float sphere(float, float, float);