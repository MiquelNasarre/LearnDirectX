#include "QuaternionMotion.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "IG_QuaternionMotion.h"

float IG_DATA::THETA = 0.f;
float IG_DATA::PHI   = 0.f;
float IG_DATA::SPEED = 0.f;

QuaternionMotion::QuaternionMotion()
	: window(640, 480, "QuaternionMotion", "", true),

	example(window.graphics, SURFACE_SHAPE(_EXPLICIT_SPHERICAL, exampleRadius))
{
	window.setFramerateLimit(60);
}

int QuaternionMotion::Run()
{
	while (window.processEvents())
		doFrame();
	return 0;
}

void QuaternionMotion::eventManager()
{

	if (Mouse::isButtonPressed(Mouse::Left) && !dragging)
	{
		dragging = true;
		lastPos = Mouse::getPosition();
		dangle = 0.f;
	}

	if (dragging && !Mouse::isButtonPressed(Mouse::Left))
		dragging = false;

	if (dragging)
	{
		Vector2i movement = Mouse::getPosition() - lastPos;
		lastPos = Mouse::getPosition();

		if (!movement)
		{
			if (axis != window.graphics.getObserver())
			{
				axis = window.graphics.getObserver();
				int wheel = Mouse::getWheel();
				dangle = 0.f;
			}

			else
				dangle += Mouse::getWheel() / 18000.f;
		}
		else
		{
			Vector3f obs = window.graphics.getObserver();
			Vector3f ex = -(obs * Vector3f(0.f, 0.f, 1.f)).normalize();
			Vector3f ey = (ex * obs).normalize();
			axis = movement.y * ex - movement.x * ey;
			dangle = movement.abs() / scale;
		}
	}
	else
		scale *= powf(1.1f, Mouse::getWheel() / 120.f);

}

void QuaternionMotion::doFrame()
{
	eventManager();

	//	Update objects

	window.graphics.updatePerspective(observer, center, scale);

	example.updateRotation(window.graphics, axis, dangle, true);

	window.setTitle("QuaternionMotion  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	//	Rendering

	window.graphics.clearBuffer(Color::Black);

	example.Draw(window.graphics);

	//	ImGui crap

	IG_QuaternionMotion::render();

	//	Push the frame to the scriin

	window.graphics.pushFrame();
}

//	Functions

float exampleRadius(float theta, float phi)
{
	return 1.f + (sinf(theta) * sinf(theta) * cosf(phi) + cosf(phi) * cosf(phi) * sinf(phi)) * sinf(5 * theta) * cosf(3 * phi) / 2.f;
}
