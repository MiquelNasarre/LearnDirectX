#include "Application.h"
#include "Keyboard.h"
#include "Mouse.h"

App::App()
	:window(640, 480, L"Hello World"), test(window.graphics)
{
	window.setFramerateLimit(60);
	//window.graphics.initTestTriangle();
	//window.graphics.bindTestTrinagle();
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

	while (!Keyboard::eventIsEmpty()) {
		Keyboard::event ev = Keyboard::popEvent();
		if (ev.type == Keyboard::event::Type::Pressed && ev.keyCode == 'W' && phi < 3.14159f / 2.f - 0.05f)
			phi += 0.05f;
		if (ev.type == Keyboard::event::Type::Pressed && ev.keyCode == 'S' && phi > -3.14159f / 2.f + 0.05f)
			phi -= 0.05f;
		if (ev.type == Keyboard::event::Type::Pressed && ev.keyCode == 'A')
			theta -= 0.05f;
		if (ev.type == Keyboard::event::Type::Pressed && ev.keyCode == 'D')
			theta += 0.05f;

	}

	window.setTitle("Hello World  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	window.graphics.clearBuffer(Color::Black);
	test.Draw(window.graphics);
	//window.graphics.drawTestTriangle(Mouse::getPosition(), scale, Vector3f(-cosf(phi) * cosf(theta), -cosf(phi) * sinf(theta), -sinf(phi)));

	window.graphics.pushFrame();

}

