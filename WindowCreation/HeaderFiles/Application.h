#pragma once

#include "Window.h"
#include "Drawable/Triangle.h"
#include "Drawable/Sphere.h"
#include "Drawable/Surface.h"
#include "Drawable/Background.h"

class App {

private:
	Window window;
	Timer timer;

	float scale = 280.f;
	Vector3f center = { 0.f,0.f,0.f };
	Vector3f observer;
	
	float* imGuiData;

	bool dragging;
	Vector2i initialDrag;
	Vector2f initialDragAngles;

	Surface Earth;
	Surface Moon;
	Background back;

public:
	App();

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