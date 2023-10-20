#include "Application.h"
#include "Keyboard.h"
#include "Mouse.h"

App::App()
	:window(640, 480, L"Hello World")
{
	window.setFramerateLimit(60);
	window.graphics.initTestTriangle();
	window.graphics.bindTestTrinagle();
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

	window.setTitle("Hello World  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");

	window.graphics.clearBuffer(Color::Black);
	window.graphics.drawTestTriangle(timer.check(), Mouse::getPosition(), scale);

	window.graphics.pushFrame();

}

