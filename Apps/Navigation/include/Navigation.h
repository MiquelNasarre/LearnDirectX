#pragma once

#include "Window.h"
#include "Drawable/Surface.h"
#include "Spherical.h"

struct IG_DATA {
	static float THETA;
	static float PHI;
	static float SPEED;

	static std::vector<Coordinates> Positions;
};

class Navigation {

private:
	Window window;

	float scale = 280.f;
	Vector3f center   = { 0.f, 0.f, 0.f };
	Vector3f observer = { 0.f,-1.f, 0.f };

	bool dragging;
	Vector2i initialDrag;
	Vector2f initialDragAngles;

	std::vector<std::unique_ptr<Surface>> Positions;
	Surface example;

public:
	Navigation();

	int Run();


	void eventManager();
	void doFrame();
};

//	Functions

template <float r>
float Radius(float, float)
{
	return r;
}