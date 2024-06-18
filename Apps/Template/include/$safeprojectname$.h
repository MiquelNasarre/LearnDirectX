#pragma once

#include "Window.h"
#include "Drawable/Surface.h"
#include <string>

//  Struct for variables shared with ImGui

struct IG_DATA {
	static float THETA;
	static float PHI;
};

//  Main class for running the Application

class $safeprojectname$ {

private:
	Window window;

	//  Variables for the FOV

	float scale = 280.f;
	Vector3f center   = { 0.f, 0.f, 0.f };
	Vector3f observer = { 0.f,-1.f, 0.f };

	//  Variables for the mouse interaction

	bool dragging;
	Vector2i lastPos;
	Vector3f axis = Vector3f(1.f, -1.f, 1.f);
	float dangle = 0.01f;
	void drag_motion();

	//  Drawables

	Surface example;

public:
	$safeprojectname$();

	int Run();

	void eventManager();
	void doFrame();
};

//	Surface Functions

float exampleRadius(float, float);