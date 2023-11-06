#include "Navigation.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "IG_Navigation.h"

float IG_DATA::THETA = 0.f;
float IG_DATA::PHI   = 0.f;
float IG_DATA::SPEED = 0.f;

Navigation::Navigation()
	: window(640, 480, "Navigation", "", true),

	example(window.graphics,_RADIAL_SPHERICAL,Radius<1>)
{
	window.setFramerateLimit(60);
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

	example.updateRotation(window.graphics, IG_DATA::THETA, IG_DATA::PHI);

	window.setTitle("Navigation  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	//	Rendering

	window.graphics.clearBuffer(Color::Black);

	example.Draw(window.graphics);

	//	ImGui crap

	IG_Navigation::render();

	//	Push the frame to the scriin

	window.graphics.pushFrame();
}