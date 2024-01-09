#pragma once

#include "Window.h"
#include "Drawable/Surface.h"
#include "Drawable/Triangle.h"

enum DRAG_TYPE
{
	RIGID_PLANE,
	RIGID_SPACE,
	DYNAMIC_PLANE,
	DYNAMIC_SPACE,
	MAGNETIC_MOUSE,
};

enum SHAPE
{
	SQUARE,
	WEIRD_SHAPE,
	OCTAHEDRON,
};

struct IG_DATA {

	static float THETA;
	static float PHI;

	static DRAG_TYPE TYPE;
	static SHAPE FIGURE;

	struct lightsource {
		bool is_on;
		_float4vector intensities;
		_float4color color;
		_float4vector position;
	};

	static lightsource* LIGHTS;
	static int UPDATE_LIGHT;
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

	//	Forced return

	bool returning = false;
	bool strict = false;

	//	Magnet motion

	bool magnetized = false;
	Vector3f magnetPos = { 1.f, 0.f, 0.f };

	Surface		shape_0;
	Triangle	shape_1;
	Triangle	shape_2;

public:
	QuaternionMotion();

	int Run();


	void eventManager();
	void doFrame();

private:
	void magneticReturn();
	void strictReturn();

	void drag_rigid_plane();
	void drag_rigid_space();
	void drag_dynamic_plane();
	void drag_dynamic_space();
	void drag_magnetic_mouse();
};

//	Functions

float exampleRadius(float, float);