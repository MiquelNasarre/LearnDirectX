#include "Navigation.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "IG_Navigation.h"

float IG_DATA::THETA = 0.f;
float IG_DATA::PHI   = 0.f;
float IG_DATA::SPEED = 0.f;
std::vector<Navigation::Waypoint>	IG_DATA::POINTS;
std::vector<Navigation::Route>		IG_DATA::ROUTES;
Window* IG_DATA::window = NULL;

using namespace PLACES;

Navigation::Navigation()
	: window(640, 480, "Navigation", "", true)
{
	Earth::create(window.graphics);
	window.setFramerateLimit(60);
	IG_DATA::window = &window;

	IG_DATA::POINTS.push_back(Waypoint(window.graphics, London, Color::White, "London"));
	IG_DATA::POINTS.push_back(Waypoint(window.graphics, Barcelona, Color::White, "Barcelona"));
	IG_DATA::POINTS.push_back(Waypoint(window.graphics, Canary, Color::White, "Canary"));
	IG_DATA::POINTS.push_back(Waypoint(window.graphics, Grenada, Color::White, "Grenada"));
	IG_DATA::POINTS.push_back(Waypoint(window.graphics, NewYork, Color::White, "New York"));
	IG_DATA::POINTS.push_back(Waypoint(window.graphics, Auckland, Color::Red, "Auckland"));

	IG_DATA::ROUTES.push_back(Route(window.graphics, { NewYork, London, Barcelona, Canary, Grenada }));
	IG_DATA::ROUTES.push_back(Route(window.graphics, { Grenada, Auckland }, Color::Blue));
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
	if (scale < 50.f)
		scale = 50.f;

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
	
	for (auto& a : IG_DATA::POINTS)
		a.point->updatePosition(window.graphics, a.pos.getVector() * ZRotationMatrix(IG_DATA::THETA) * XRotationMatrix(IG_DATA::PHI));

	for (auto& b : IG_DATA::ROUTES)
		b.route->updateRotation(window.graphics, IG_DATA::THETA, IG_DATA::PHI);

	window.setTitle("Navigation  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	//	Rendering

	window.graphics.clearBuffer(Color::Black);

	Earth::Draw(window.graphics);

	for (auto& a : IG_DATA::POINTS)
		a.point->Draw(window.graphics);

	for (auto& b : IG_DATA::ROUTES)
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
	SURFACE_COLORING sc(tex, false);
	unique.sphere = std::make_unique<Surface>(gfx, SURFACE_SHAPE(_PARAMETRIC_SPHERICAL, EarthShape), &sc);

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

Navigation::Waypoint::Waypoint(Graphics& gfx, const Coordinate& coor, Color color, std::string na)
	: pos{coor}
{
	name = (char*)calloc(20, sizeof(char));
	for (UINT i = 0; i <= na.size(); i++)
		name[i] = na.c_str()[i];
	point = std::make_unique<Point>(gfx, Vector3f(), 7.f, color);
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
