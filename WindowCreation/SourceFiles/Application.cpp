#include "Application.h"
#include "Keyboard.h"
#include "Mouse.h"

App::App()
	:window(640, 480, L"Hello World"), surface(window.graphics, SincFunction, Vector2f(-4.f, -4.f), Vector2f(4.f, 4.f), 200u, 200u)
{

	window.setFramerateLimit(60);
	timer.reset();

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

	if (Keyboard::isKeyPressed('W') && phi < 3.14159f / 2.f - 0.01f)
		phi += 0.01f;
	if (Keyboard::isKeyPressed('S') && phi > -3.14159f / 2.f + 0.01f)
		phi -= 0.01f;
	if (Keyboard::isKeyPressed('A'))
		theta -= 0.01f;
	if (Keyboard::isKeyPressed('D'))
		theta += 0.01f;

	window.graphics.updatePerspective(Vector3f(-cosf(phi) * cosf(theta), -cosf(phi) * sinf(theta), -sinf(phi)), scale);


	window.setTitle("Hello World  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	window.graphics.clearBuffer(Color::Black);

	surface.Draw(window.graphics);

	window.graphics.pushFrame();

}

//	Surface functions

float SincFunction(float x, float y)
{
	if (x == 0.f && y == 0.f)
		return 3;
	return sinf(3 * sqrtf(x * x + y * y)) / sqrtf(x * x + y * y);
}
