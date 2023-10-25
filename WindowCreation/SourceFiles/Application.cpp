#include "Application.h"
#include "Keyboard.h"
#include "Mouse.h"

#include "iGManager.h"

App::App()
	:window(640, 480, L"Hello World"), test(window.graphics), back(window.graphics, "Resources/Background.jpg")
{

	window.setFramerateLimit(60);
	timer.reset();

	//surfaces.push_back(std::make_unique<Surface>(window.graphics, _FUNCTION_RADIUS_LAT_LONG, weirdRadius, 200u, 200u));
	//surfaces.push_back(std::make_unique<Surface>(window.graphics, _FUNCTION_EXPLICIT, SincFunction, Vector2f(-4.f, -4.f), Vector2f(4.f, 4.f), 200u, 200u));
	//surfaces.push_back(std::make_unique<Surface>(window.graphics, _FUNCTION_RADIUS_ICOSPHERE, constantRadius, 5u));

}

int App::Run()
{
	while (window.processEvents())
		doFrame();
	return 0;
}

void App::doFrame()
{
	scale *= powf(1.1f, Mouse::getWheel() / 120.f);

	if (Keyboard::isKeyPressed('W'))
		center.y += 0.01f;
	if (Keyboard::isKeyPressed('S'))
		center.y -= 0.01f;
	if (Keyboard::isKeyPressed('A'))
		center.x -= 0.01f;
	if (Keyboard::isKeyPressed('D'))
		center.x += 0.01f;
	if (Keyboard::isKeyPressed('R'))
		center.z += 0.01f;
	if (Keyboard::isKeyPressed('F'))
		center.z -= 0.01f;

	if (Mouse::isButtonPressed(Mouse::Left) && !dragging) {
		dragging = true;
		initialDrag = Mouse::getPosition();
		initialDragAngles = { theta,phi };
	}
	if (dragging && !Mouse::isButtonPressed(Mouse::Left))
		dragging = false;

	if (dragging) {
		Vector2i movement = Mouse::getPosition() - initialDrag;
		float temp = initialDragAngles.x - 2.f * (float)movement.x / scale;
		velocity = temp - theta;
		theta = temp;
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
	else {
		if (velocity > 0.1f)
			velocity -= velocity/100.f;
		if (velocity < -0.1f)
			velocity -= velocity/100.f;
		theta += velocity;
	}

	window.graphics.updatePerspective({ 0.f,-1.f,0.f }/*Vector3f(-cosf(phi) * cosf(theta), -cosf(phi) * sinf(theta), -sinf(phi))*/, center, scale);


	window.setTitle("Hello World  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	window.graphics.clearBuffer(Color::Black);

	for (std::unique_ptr<Surface>& s : surfaces) {
		s->updateRotation(window.graphics, theta, phi);
		s->Draw(window.graphics);
	}
	test.updateRotation(window.graphics, theta, phi);
	test.Draw(window.graphics);
	back.Draw(window.graphics);

	//	ImGui crap

	iGManager::render();

	//	Push the frame to the scriin

	window.graphics.pushFrame();

}

//	Surface functions

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

float weirdRadius(float theta, float phi)
{
	return 1.f + (sinf(theta) * sinf(theta) * cosf(phi) + cosf(phi) * cosf(phi) * sinf(phi)) * sinf(5*theta) * cosf(3*phi) / 2.f;
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
