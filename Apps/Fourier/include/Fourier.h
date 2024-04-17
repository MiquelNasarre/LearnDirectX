#pragma once

#include "Window.h"
#include "Drawable/Surface.h"

struct IG_DATA {
	static float THETA;
	static float PHI;
	static float SPEED;

	static int L;
	static int M;

	static bool UPDATE;
};

class Fourier {

private:
	Window window;

	float scale = 280.f;
	Vector3f center = { 0.f, 0.f, 0.f };
	Vector3f observer = { 0.f,-1.f, 0.f };

	Vector3f axis = Vector3f(1.f, -1.f, 1.f);
	float dangle = 0.01f;

	Vector2i lastPos;
	bool dragging;

	//	Forced return

	bool returning = false;
	bool strict = false;

	//	Magnet motion

	bool magnetized = false;
	Vector3f magnetPos = { 1.f, 0.f, 0.f };


	void drag_dynamic_space();
	void magneticReturn();
	void strictReturn();

	// Surfaces

	Surface positive;
	Surface negative;

public:
	Fourier();

	int Run();


	void eventManager();
	void doFrame();
};

//	Functions

float exampleRadius(float, float);

Vector3f Ylm(float phi, float theta);