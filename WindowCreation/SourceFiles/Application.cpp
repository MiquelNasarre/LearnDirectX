#include "Application.h"

App::App()
	:window(640, 480, L"Hello World")
{
	window.setFramerateLimit(60);
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
	window.graphics.clearBuffer(0.f, 0.f, 0.f,1.f);
	window.graphics.drawTestTriangle();
	window.graphics.pushFrame();

}

