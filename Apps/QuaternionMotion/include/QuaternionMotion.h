#pragma once

#include "Window.h"
#include "Drawable/Surface.h"

struct IG_DATA {
	static float THETA;
	static float PHI;
	static float SPEED;
};

class QuaternionMotion {

private:
	Window window;

	float scale = 280.f;
	Vector3f center   = { 0.f, 0.f, 0.f };
	Vector3f observer = { 0.f,-1.f, 0.f };

	Vector3f axis = Vector3f(1.f, -1.f, 1.f);
	float dangle = 0.01f;

	Vector2i lastPos;
	bool dragging;

	Surface example;

public:
	QuaternionMotion();

	int Run();


	void eventManager();
	void doFrame();
};

//	Functions

float exampleRadius(float, float);