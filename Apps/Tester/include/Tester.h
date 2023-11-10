#pragma once

#include "Window.h"

#include "Drawable/Surface.h"
#include "Drawable/PointLight.h"
#include "Drawable/Curve.h"

struct IG_DATA {
	static float THETA;
	static float PHI;
};

class Tester {

private:
	Window window;

	//	Add drawables you want to test here

	Surface surf;
	PointLight light;
	Curve curve;

	Surface Klein;

	//	...

	float scale = 250.f;
	float theta = 0.f;
	float phi = 0.f;
	Vector3f center = {};

	bool dragging = false;
	Vector2i initialDrag;
	Vector2f initialDragAngles;

public:
	Tester();
	int Run();

	void eventManager();
	void doFrame();
};

//	Surface functions

Vector3f curveF(float);

Vector3f KleinBottle(float, float);

float curveX(float);

float curveY(float);

float curveZ(float);

float SincFunction(float, float);

float constantRadius(float, float);

float constantRadius03(float, float);

float weirdRadius(float, float);

float returnX(float, float);

float returnY(float, float);

float sphere(float, float, float);