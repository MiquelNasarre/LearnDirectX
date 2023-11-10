#include "Navigation.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "IG_Navigation.h"

float IG_DATA::THETA = 0.f;
float IG_DATA::PHI   = 0.f;
float IG_DATA::SPEED = 0.f;

#define pl Coordinate::PLACES

Navigation::Navigation()
	: window(640, 480, "Navigation", "", true)
{
	Earth::create(window.graphics);
	window.setFramerateLimit(60);

	points.push_back(Point(window.graphics, pl::London));
	points.push_back(Point(window.graphics, pl::Barcelona));
	points.push_back(Point(window.graphics, pl::Canary));
	points.push_back(Point(window.graphics, pl::Grenada));
	points.push_back(Point(window.graphics, pl::NewYork));
	points.push_back(Point(window.graphics, pl::Auckland, Color::Red));

	routes.push_back(Route(window.graphics, std::vector<Coordinate>{ pl::NewYork, pl::London, pl::Barcelona, pl::Canary, pl::Grenada }));
	routes.push_back(Route(window.graphics, std::vector<Coordinate>{ pl::Grenada, pl::Auckland }, Color::Blue));
}

int Navigation::Run()
{
	while (window.processEvents())
		doFrame();
	return 0;
}

void Navigation::eventManager()
{
	//	Keyboard and Mouse events

	scale *= powf(1.1f, Mouse::getWheel() / 120.f);

	if (Keyboard::isKeyPressed('W'))
		center.y += 0.02f;
	if (Keyboard::isKeyPressed('S'))
		center.y -= 0.02f;
	if (Keyboard::isKeyPressed('A'))
		center.x -= 0.02f;
	if (Keyboard::isKeyPressed('D'))
		center.x += 0.02f;
	if (Keyboard::isKeyPressed('R'))
		center.z += 0.02f;
	if (Keyboard::isKeyPressed('F'))
		center.z -= 0.02f;

	if (Mouse::isButtonPressed(Mouse::Left) && !dragging) {
		dragging = true;
		initialDrag = Mouse::getPosition();
		initialDragAngles = { IG_DATA::THETA,IG_DATA::PHI };
	}
	if (dragging && !Mouse::isButtonPressed(Mouse::Left))
		dragging = false;

	if (dragging) {
		Vector2i movement = Mouse::getPosition() - initialDrag;
		float temp = initialDragAngles.x - 2.f * (float)movement.x / scale;
		IG_DATA::SPEED = -10.f * (temp - IG_DATA::THETA);
		IG_DATA::THETA = temp;
		IG_DATA::PHI = initialDragAngles.y + 2.f * (float)movement.y / scale;
		if (IG_DATA::PHI > pi / 2.f) {
			IG_DATA::PHI = pi / 2.f - 0.0001f;
			initialDrag.y = Mouse::getPosition().y - int((pi / 2.f - initialDragAngles.y) * scale / 2.f);
		}
		if (IG_DATA::PHI < -pi / 2.f) {
			IG_DATA::PHI = -pi / 2.f + 0.0001f;
			initialDrag.y = Mouse::getPosition().y - int((-pi / 2.f - initialDragAngles.y) * scale / 2.f);
		}
	}
	else {
		if (IG_DATA::SPEED > 1.f) {
			IG_DATA::SPEED *= 0.99f;
			if (IG_DATA::SPEED < 1.f)
				IG_DATA::SPEED = 1.f;
		}
		if (IG_DATA::SPEED < -1.f) {
			IG_DATA::SPEED *= 0.99f;
			if (IG_DATA::SPEED > -1.f)
				IG_DATA::SPEED = -1.f;
		}

		IG_DATA::THETA += -IG_DATA::SPEED / 10.f;
	}
}

void Navigation::doFrame()
{
	eventManager();

	//	Update objects

	window.graphics.updatePerspective(observer, center, scale);

	Earth::update(window.graphics, IG_DATA::THETA, IG_DATA::PHI);
	
	for (auto& a : points)
		a.point->updateRotation(window.graphics, IG_DATA::THETA, IG_DATA::PHI);

	for (auto& b : routes)
		b.route->updateRotation(window.graphics, IG_DATA::THETA, IG_DATA::PHI);

	window.setTitle("Navigation  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	//	Rendering

	window.graphics.clearBuffer(Color::Black);

	Earth::Draw(window.graphics);

	for (auto& a : points)
		a.point->Draw(window.graphics);

	for (auto& b : routes)
		b.route->Draw(window.graphics);

	//	ImGui crap

	IG_Navigation::render();

	//	Push the frame to the scriin

	window.graphics.pushFrame();
}

//	Earth stuff

Navigation::Earth Navigation::Earth::unique;

void Navigation::Earth::create(Graphics& gfx)
{
	Texture tex(gfx, "resources/EarthTexture.jpg");
	unique.sphere = std::make_unique<Surface>(gfx, _PARAMETRIC_SPHERICAL, EarthShape, SURFACE_COLORING( tex, false));

	for (UINT i = 0; i < numMer; i++)
		unique.meridians.push_back(std::make_unique<Curve>(gfx, Meridian, Coordinate(0.f, 360.f / numMer * i, DEGREES), Vector2f{ 0.f,2.f * pi }, 1000u, Color::Gray));

	for (UINT i = 0; i < numPar; i++)
		unique.parallels.push_back(std::make_unique<Curve>(gfx, Parallel, Coordinate(-90.f + 180.f * i / numPar, 0.f, DEGREES), Vector2f{ 0.f,2.f * pi }, 1000u, Color::Gray));

}

void Navigation::Earth::Draw(Graphics& gfx)
{
	unique.sphere->Draw(gfx);

	for (auto& a : unique.meridians)
		a->Draw(gfx);

	for (auto& b : unique.parallels)
		b->Draw(gfx);
}

void Navigation::Earth::update(Graphics& gfx, float theta, float phi)
{
	unique.sphere->updateRotation(gfx, theta, phi);

	for (auto& a : unique.meridians)
		a->updateRotation(gfx, theta, phi);

	for (auto& b : unique.parallels)
		b->updateRotation(gfx, theta, phi);
}

Navigation::Point::Point(Graphics& gfx, const Coordinate& coor, Color color)
{
	pos = coor;
	point = std::make_unique<Surface>(gfx, _PARAMETRIC, PointGen, coor, SURFACE_COLORING(color, false), false, Vector2f(0.f, -pi / 2.f), Vector2f(2.f * pi, pi / 2.f));
}

Navigation::Route::Route(Graphics& gfx, std::vector<Coordinate> coor, Color color)
{
	Coor = coor;
	route = std::make_unique<Curve>(gfx, RouteGen, coor, Vector2f(0.f, coor.size() - 1.f), 1000u, color);
}

//	Drawable generation functions

Vector3f Meridian(float t, const Coordinate& coor)
{
	Vector3f North(0.f, 0.f, 1.f);
	Vector3f Ecuator(cosf(coor.Longitude), sinf(coor.Longitude), 0.f);
	return 1.002f * (North * cosf(t) + Ecuator * sinf(t));
}

Vector3f Parallel(float t, const Coordinate& coor)
{
	return 1.002f * Vector3f(cosf(coor.latitude) * cosf(t), cosf(coor.latitude) * sinf(t), sinf(coor.latitude));
}

Vector3f PointGen(float theta, float phi, const Coordinate& coor)
{
	return 0.01f * Vector3f(cos(phi) * cos(theta), cos(phi) * sin(theta), sin(phi)) + coor.getVector();
}

Vector3f RouteGen(float t, const std::vector<Coordinate>& coor)
{
	int t0 = int(t);
	int t1 = t0 + 1;
	if (t1 == coor.size())
		t1--;
	Vector3f coor0 = coor[t0].getVector();
	Vector3f coor1 = coor[t1].getVector();
	t -= int(t);
	Vector3f dir = (coor0 * coor1 * coor0).normalize();
	float angle = acosf(coor0 ^ coor1);
	return 1.002f * (coor0 * cos(t * angle) + dir * sin(t * angle));
}

Vector3f EarthShape(float theta, float phi)
{
	return Vector3f(theta + pi, phi, 1.f);
}
