#pragma once

#include "Window.h"
#include "Drawable/Triangle.h"
#include "Drawable/Sphere.h"
#include "Drawable/Surface.h"

class App {

private:
	Window window;
	Timer timer;

	Surface surface;

	float scale = 280.f;
	float theta = 3.14159f / 2.f;
	float phi = 0.f;
	Vector3f center = { 0.f,0.f,0.f };
	float velocity = 0.f;

	bool dragging;
	Vector2i initialDrag;
	Vector2f initialDragAngles;
public:
	App();

	int Run();
	void doFrame();
};

//	Surface functions

float SincFunction(float, float);

float constantRadius(float, float);

float weirdRadius(float, float);

float returnX(float, float);
 
float returnY(float, float);