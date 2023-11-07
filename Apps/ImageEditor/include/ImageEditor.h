#pragma once

#include "Window.h"
#include "Drawable/Surface.h"

struct IG_DATA {
	static float THETA;
	static float PHI;
	static float SPEED;
};

class ImageEditor {

private:
	Window window;

	float scale = 280.f;
	Vector3f center   = { 0.f, 0.f, 0.f };
	Vector3f observer = { 0.f,-1.f, 0.f };

	bool dragging;
	Vector2i initialDrag;
	Vector2f initialDragAngles;

	Surface example;

public:
	ImageEditor();

	int Run();


	void eventManager();
	void doFrame();
};

//	Functions

float exampleRadius(float, float);