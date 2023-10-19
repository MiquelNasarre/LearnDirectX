#include "Application.h"
#include "Keyboard.h"
#include "Mouse.h"

App::App()
	:window(640, 480, L"Hello World")
{
	window.setFramerateLimit(60);
	window.graphics.initTestTriangle();
	window.graphics.initSettings();
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

	window.setTitle("Hello World  -  " + std::to_string(int(std::round(window.getFramerate()))) + "fps");
	window.graphics.clearBuffer(Color::Black);
	window.graphics.drawTestTriangle(timer.check(), Mouse::getPosition());
	window.graphics.pushFrame();

}

