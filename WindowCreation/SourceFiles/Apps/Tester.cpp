#include "Apps/Tester.h"
#include "ImGui/IG_Tester.h"
#include "Keyboard.h"
#include "Mouse.h"

float IG_DATA_TESTER::THETA = pi / 2.f;
float IG_DATA_TESTER::PHI = 0.f;

Tester::Tester()
	:window(640, 480, "Plotter"),
	surf(window.graphics, _RADIAL_SPHERICAL, weirdRadius),
	light(window.graphics, Color::Black, { 3.f , 0.f , 2.f }, 1.f),
	curve(window.graphics, curveF, { -10,10 }, 200)
{
	window.setFramerateLimit(60);
	srand(143452);
	surf.clearLights(window.graphics);
}

int Tester::Run()
{
	while (window.processEvents())
		doFrame();
	return 0;
}

void Tester::eventManager()
{
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
		initialDragAngles = { theta, phi };
	}
	if (dragging && !Mouse::isButtonPressed(Mouse::Left))
		dragging = false;

	if (dragging) {
		Vector2i movement = Mouse::getPosition() - initialDrag;

		theta = initialDragAngles.x - 2.f * (float)movement.x / scale;
		phi = initialDragAngles.y + 2.f * (float)movement.y / scale;

		if (phi > pi / 2.f) {
			phi = pi / 2.f - 0.0001f;
			initialDrag.y = Mouse::getPosition().y - int((pi / 2.f - initialDragAngles.y) * scale / 2.f);
		}
		if (phi < -pi / 2.f) {
			phi = -pi / 2.f + 0.0001f;
			initialDrag.y = Mouse::getPosition().y - int((-pi / 2.f - initialDragAngles.y) * scale / 2.f);
		}
	}

	if (IG_DATA_TESTER::PHI > 3.1415f / 2.f)
		IG_DATA_TESTER::PHI = 3.1415f / 2.f;

	if (IG_DATA_TESTER::PHI < -3.1415f / 2.f)
		IG_DATA_TESTER::PHI = -3.1415f / 2.f;
}

void Tester::doFrame()
{
	eventManager();

	window.graphics.clearBuffer(Color::Black);

	window.graphics.updatePerspective({ -cosf(IG_DATA_TESTER::PHI) * cosf(IG_DATA_TESTER::THETA), -cosf(IG_DATA_TESTER::PHI) * sinf(IG_DATA_TESTER::THETA), -sinf(IG_DATA_TESTER::PHI) }, center, scale);
	light.update(window.graphics);

	static int count = 0;
	if (!(++count % 100)) {
		Color col(rand() % 255, rand() % 255, rand() % 255);
		float rad = rand() / 32728.f;
		light.updateColor(window.graphics, col);
		light.updateRadius(window.graphics, rad);
		surf.updateLight(window.graphics, 0, { 3 * rad, rad }, col, { 3.f , 0.f , 2.f });
	}

	surf.updateRotation(window.graphics, theta, phi);
	curve.updateRotation(window.graphics, theta, phi);
	surf.Draw(window.graphics);
	light.Draw(window.graphics);
	curve.Draw(window.graphics);

	IG_Tester::render();
	window.graphics.pushFrame();
}

//	Surface functions

Vector3f curveF(float t)
{
	return Vector3f(cosf(t), sinf(t), t/5.f);
}

float SincFunction(float x, float y)
{
	if (x == 0.f && y == 0.f)
		return 3;
	return sinf(3 * sqrtf(x * x + y * y)) / sqrtf(x * x + y * y);
}

float constantRadius(float, float)
{
	return 1.f;
}

float constantRadius03(float, float)
{
	return 0.3f;
}

float weirdRadius(float theta, float phi)
{
	return 1.f + (sinf(theta) * sinf(theta) * cosf(phi) + cosf(phi) * cosf(phi) * sinf(phi)) * sinf(5 * theta) * cosf(3 * phi) / 2.f;
}

float returnX(float x, float)
{
	return x;
}

float returnY(float, float y)
{
	return y;
}

float sphere(float x, float y, float z)
{
	return x * x + y * y + z * z - 1;
}
