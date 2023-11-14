#pragma once

#include "Window.h"
#include "Drawable/Surface.h"
#include "Drawable/Curve.h"
#include "Drawable/Point.h"
#include "Coordinate.h"

class Navigation {
	class Earth
	{
		std::unique_ptr<Surface> sphere;
		std::vector<std::unique_ptr<Curve>> meridians;
		std::vector<std::unique_ptr<Curve>> parallels;

		static const UINT numMer = 10u;
		static const UINT numPar = 10u;

		Earth() = default;
		static Earth unique;

	public:

		static void create(Graphics& gfx);
		static void Draw(Graphics& gfx);
		static void update(Graphics& gfx, float theta, float phi);
	};

private:
	Window window;

	float scale = 400.f;
	Vector3f center   = { 0.f, 0.f, 0.f };
	Vector3f observer = { 0.f,-1.f, 0.f };

	bool dragging;
	Vector2i initialDrag;
	Vector2f initialDragAngles;

public:

	struct Waypoint
	{
		Coordinate pos;
		std::unique_ptr<Point> point;
		char* name;
		float rad = 7.f;
		float color[4] = { 1.f,1.f,1.f,1.f };

		Waypoint(Graphics& gfx, const Coordinate& coor, Color color = Color::White, std::string name = "");
	};

	struct Route
	{
		std::vector<Coordinate> Coor;
		std::unique_ptr<Curve> route;

		Route(Graphics& gfx, std::vector<Coordinate> coor, Color color = Color::White);
	};

	Navigation();

	int Run();


	void eventManager();
	void doFrame();
};

struct IG_DATA {
	static Window* window;

	static float THETA;
	static float PHI;
	static float SPEED;

	static std::vector<Navigation::Waypoint> POINTS;
	static std::vector<Navigation::Route> ROUTES;
};

//	Drawable generation functions

Vector3f Meridian(float t, const Coordinate& coor);
Vector3f Parallel(float t, const Coordinate& coor);
Vector3f PointGen(float theta, float phi, const Coordinate& coor);
Vector3f RouteGen(float t, const std::vector<Coordinate>& coor);
Vector3f EarthShape(float theta, float phi);
